#include <JobSystem/JobSystem.hpp>
#include <JobSystem/ThreadWorker.hpp>

namespace shkyera {

static constexpr auto StopWhenDone = false;

ThreadWorker::ThreadWorker() : _thread(&ThreadWorker::run, this) {}

ThreadWorker::~ThreadWorker() {
  if (_thread.joinable()) {
    _thread.join();
  }
}

void ThreadWorker::run() {
  auto& jobSystem = JobSystem::getInstance();
  while (_stop == false) {
    const JobExecutor job = jobSystem.pull();
    job();
  }
}

}  // namespace shkyera
