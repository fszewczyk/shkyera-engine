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

/**
 * Singleton class managing job scheduling and execution.
 * Provides a thread-safe job system for parallel task execution with resource dependencies.
 * Jobs can be configured with dependencies, resource requirements, and thread affinity.
 */
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

  /**
   * Submits a job to the system for execution. The job will be scheduled based on its dependencies
   * and resource requirements.
   * 
   * @param job The job to be submitted
   * @return Handle to the submitted job
   */
  JobHandle submit(Job&& job);

  /**
   * Checks if a job is currently scheduled in the system. A job is considered scheduled
   * if it has been submitted and the user did not wait() for all jobs to complete.
   * 
   * @param handle The job handle to check
   * @return true if the job is scheduled, false otherwise
   */
  bool isScheduled(JobHandle handle) const;

  /**
   * Gets the next available job that can be executed. A job is available if all its dependencies
   * are satisfied and its resource requirements don't conflict with currently running jobs.
   * 
   * @return Optional containing the next job handle if available
   */
  std::optional<JobHandle> getJob();

  /**
   * Marks a job as completed and updates resource states. This will release any resources
   * that were being used by the job.
   * 
   * @param handle The handle of the completed job
   */
  void markJobAsDone(JobHandle handle);

  std::mutex _mutex;
  std::condition_variable _cv;
  JobHandle _nextJobHandle;

  TypeSet _resourcesRead;
  TypeSet _resourcesWritten;

  friend class SparseSet<Job>;
  SparseSet<Job> _jobs;

 public:
  /**
   * Gets the singleton instance of the job system. This is the only way to access
   * the job system instance.
   * 
   * @return Reference to the job system instance
   */
  static JobSystem& getInstance();

  /**
   * Pulls the next available job for execution.
   * 
   * @return Executor function for the next available job
   */
  JobExecutor pull();

  /**
   * Waits for all scheduled jobs to complete. This method blocks until all jobs
   * in the system have finished execution.
   */
  void wait();

  /**
   * Builder class for constructing and configuring jobs.
   */
  class JobBuilder {
   public:
    /**
     * Constructs a job builder with the given executor function. The function will be
     * executed when the job runs.
     * 
     * @param function The function to be executed by the job
     */
    JobBuilder(JobExecutor function);
    ~JobBuilder() = default;
    JobBuilder(const JobBuilder&) = delete;
    JobBuilder& operator=(const JobBuilder&) = delete;
    JobBuilder(JobBuilder&&) = delete;
    JobBuilder& operator=(JobBuilder&&) = delete;

    /**
     * Adds a dependency to the job. The job will not execute until all its
     * dependencies have completed.
     * 
     * @param handle Handle of the job this job depends on
     * @return Reference to this builder for chaining
     */
    JobBuilder& dependsOn(JobHandle handle);

    /**
     * Specifies a resource that the job needs to read. The job will not execute
     * if any other job is currently writing to this resource.
     * 
     * @tparam ResourceType Type of the resource to read
     * @return Reference to this builder for chaining
     */
    template <typename ResourceType>
    JobBuilder& readResource() {
      static_assert(std::is_base_of_v<ResourceTag, ResourceType>, "ResourceType is not tagged with ResourceTag");
      _job.readResources.emplace(TypeInfo<ResourceType>::ID);
      _job.mainThread |= OnlyMainThread<ResourceType>;
      return *this;
    }

    /**
     * Specifies a resource that the job needs to write. The job will not execute
     * if any other job is currently reading or writing to this resource.
     * 
     * @tparam ResourceType Type of the resource to write
     * @return Reference to this builder for chaining
     */
    template <typename ResourceType>
    JobBuilder& writeResource() {
      static_assert(std::is_base_of_v<ResourceTag, ResourceType>, "ResourceType is not tagged with ResourceTag");
      _job.writeResources.emplace(TypeInfo<ResourceType>::ID);
      _job.mainThread |= OnlyMainThread<ResourceType>;
      return *this;
    }

    /**
     * Specifies a resource by ID that the job needs to read.
     * 
     * @param typeId ID of the resource to read
     * @return Reference to this builder for chaining
     */
    JobBuilder& readResource(TypeID typeId) {
      _job.readResources.emplace(typeId);
      return *this;
    }

    /**
     * Specifies a resource by ID that the job needs to write. This is used for
     * dynamic resource types that aren't known at compile time.
     * 
     * @param typeId ID of the resource to write
     * @return Reference to this builder for chaining
     */
    JobBuilder& writeResource(TypeID typeId) {
      _job.writeResources.emplace(typeId);
      return *this;
    }

    /**
     * Forces the job to run on the main thread.
     * 
     * @return Reference to this builder for chaining
     */
    void useMainThread() { _job.mainThread |= true; }

    /**
     * Submits the configured job to the system.
     * 
     * @return Handle to the submitted job
     */
    JobHandle submit();

   private:
    Job _job;
  };
};

using JobBuilder = JobSystem::JobBuilder;

}  // namespace shkyera
