#include <AssetManager/Asset.hpp>
#include <Common/Assert.hpp>
#include <JobSystem/JobSystem.hpp>

#include <algorithm>
#include <thread>
#include <vector>

namespace shkyera {

static const std::thread::id MAIN_THREAD_ID = std::this_thread::get_id();
thread_local const std::thread::id THREAD_ID = std::this_thread::get_id();

JobExecutor JobSystem::pull() {
  JobExecutor executor;

  {
    std::optional<JobHandle> handle;
    std::unique_lock lock(_mutex);
    _cv.wait(lock, [this, &handle] {
      handle = getJob();
      return handle.has_value();
    });

    auto& job = _jobs.get(*handle);

    job.status = JobStatus::RUNNING;
    executor = [this, h = *handle, f = std::move(job.function)]() {
      f();
      markJobAsDone(h);
      _cv.notify_all();
    };
  }

  _cv.notify_all();

  return executor;
}

void JobSystem::wait() {
  while (true) {
    JobExecutor executor;
    {
      std::unique_lock lock(_mutex);
      std::optional<JobHandle> handle{};
      bool jobsLeft{true};
      _cv.wait(lock, [this, &handle, &jobsLeft] {
        handle = getJob();
        const auto foundJob = handle.has_value();
        jobsLeft = std::any_of(_jobs.begin(), _jobs.end(),
                               [](const auto& handleAndJob) { return handleAndJob.second.status != JobStatus::DONE; });

        return foundJob || !jobsLeft;
      });

      if (!jobsLeft) {
        break;
      }

      SHKYERA_ASSERT(handle.has_value(), "Job Handle has no value. Cannot schedule job");

      auto& job = _jobs.get(*handle);

      job.status = JobStatus::RUNNING;
      _resourcesRead.insert(job.readResources.begin(), job.readResources.end());
      _resourcesWritten.insert(job.writeResources.begin(), job.writeResources.end());

      executor = [this, h = *handle, f = std::move(job.function)]() {
        f();
        markJobAsDone(h);
        _cv.notify_all();
      };
    }
    executor();
  }

  _jobs.clear();
}

JobHandle JobSystem::submit(Job&& job) {
  SHKYERA_ASSERT(std::ranges::all_of(job.dependsOn, [this](auto handle) { return isScheduled(handle); }),
                 "Job was not scheduled. Cannot schedule a Job after it.");

  JobHandle handle = ++_nextJobHandle;

  {
    std::scoped_lock lock(_mutex);
    _jobs.add(handle, std::move(job));
  }

  _cv.notify_all();

  return handle;
}

JobSystem& JobSystem::getInstance() {
  static JobSystem instance;
  return instance;
}

bool JobSystem::isScheduled(JobHandle handle) const {
  return _jobs.contains(handle);
}

std::optional<JobHandle> JobSystem::getJob() {
  const auto isMainThread = MAIN_THREAD_ID == THREAD_ID;

  for (const auto& [handle, job] : _jobs) {
    if (job.mainThread && !isMainThread) {
      continue;
    }

    if (job.status != JobStatus::PENDING) {
      continue;
    }

    if (std::ranges::any_of(job.dependsOn,
                            [this](const auto& handle) { return _jobs.get(handle).status != JobStatus::DONE; })) {
      continue;
    }

    if (std::ranges::any_of(job.readResources,
                            [this](const auto& resource) { return _resourcesWritten.contains(resource); })) {
      continue;
    }

    if (std::ranges::any_of(job.writeResources, [this](const auto& resource) {
          return _resourcesRead.contains(resource) || _resourcesWritten.contains(resource);
        })) {
      continue;
    }

    return handle;
  }

  return std::nullopt;
}

void JobSystem::markJobAsDone(JobHandle handle) {
  std::scoped_lock lock(_mutex);

  SHKYERA_ASSERT(_jobs.contains(handle), "Job Handle {} cannot be marked as done, because it's not stored", handle);

  auto& job = _jobs.get(handle);
  job.status = JobStatus::DONE;
  std::erase_if(_resourcesWritten, [&job](const auto& resource) { return job.writeResources.contains(resource); });
  std::erase_if(_resourcesRead, [&job](const auto& resource) { return job.readResources.contains(resource); });
}

JobBuilder::JobBuilder(JobExecutor func) {
  _job.function = std::move(func);
}

JobBuilder& JobBuilder::dependsOn(JobHandle handle) {
  _job.dependsOn.push_back(handle);
  return *this;
}

JobHandle JobBuilder::submit() {
  return JobSystem::getInstance().submit(std::move(_job));
}

}  // namespace shkyera
