#pragma once

#include <Common/TypeInfo.hpp>
#include <ECS/Entity.hpp>
#include <ECS/SparseSet.hpp>
#include <JobSystem/Requirements.hpp>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <type_traits>
#include <vector>

namespace shkyera {

using JobHandle = Entity;
using JobExecutor = std::function<void()>;

class JobSystem {
 private:
  enum class JobStatus : uint8_t { PENDING, RUNNING, DONE };

  struct Job {
    Job() = default;
    Job(Job&&) noexcept = default;
    Job& operator=(Job&&) noexcept = default;
    Job(const Job&) = delete;
    Job& operator=(const Job&) = delete;

    bool mainThread{false};
    JobStatus status{JobStatus::PENDING};
    std::vector<JobHandle> dependsOn{};
    JobExecutor function{};
    TypeSet readResources{};
    TypeSet writeResources{};
  };

  JobSystem() = default;

  JobHandle submit(Job&& job);

  bool isScheduled(JobHandle handle) const;
  std::optional<JobHandle> getJob();
  void markJobAsDone(JobHandle handle);

  std::mutex _mutex;
  std::condition_variable _cv;
  JobHandle _nextJobHandle;

  TypeSet _resourcesRead;
  TypeSet _resourcesWritten;

  friend class SparseSet<Job>;
  SparseSet<Job> _jobs;

 public:
  static JobSystem& getInstance();

  JobExecutor pull();
  void wait();

  class JobBuilder {
   public:
    JobBuilder(JobExecutor function);
    ~JobBuilder() = default;
    JobBuilder(const JobBuilder&) = delete;
    JobBuilder& operator=(const JobBuilder&) = delete;
    JobBuilder(JobBuilder&&) = delete;
    JobBuilder& operator=(JobBuilder&&) = delete;

    JobBuilder& dependsOn(JobHandle handle);

    template <typename ResourceType>
    JobBuilder& readResource() {
      static_assert(std::is_base_of_v<ResourceTag, ResourceType>, "ResourceType is not tagged with ResourceTag");
      _job.readResources.emplace(TypeInfo<ResourceType>::ID);
      _job.mainThread |= OnlyMainThread<ResourceType>;
      return *this;
    }

    template <typename ResourceType>
    JobBuilder& writeResource() {
      static_assert(std::is_base_of_v<ResourceTag, ResourceType>, "ResourceType is not tagged with ResourceTag");
      _job.writeResources.emplace(TypeInfo<ResourceType>::ID);
      _job.mainThread |= OnlyMainThread<ResourceType>;
      return *this;
    }

    JobBuilder& readResource(TypeID typeId) {
      _job.readResources.emplace(typeId);
      return *this;
    }

    JobBuilder& writeResource(TypeID typeId) {
      _job.writeResources.emplace(typeId);
      return *this;
    }

    void useMainThread() { _job.mainThread |= true; }

    JobHandle submit();

   private:
    Job _job;
  };
};

using JobBuilder = JobSystem::JobBuilder;

}  // namespace shkyera
