#pragma once

#include <thread>

namespace shkyera {

/**
 * Worker thread class for executing jobs from the job system.
 * Manages a dedicated thread that continuously pulls and executes jobs from the job system.
 * The worker runs in a loop until stopped, processing jobs as they become available.
 */
class ThreadWorker {
 public:
  /**
   * Constructs a new worker thread and starts it. The thread will begin pulling
   * and executing jobs immediately.
   */
  ThreadWorker();

  /**
   * Destroys the worker thread and ensures proper cleanup. This will stop the
   * worker thread and wait for it to finish.
   */
  ~ThreadWorker();

  /**
   * Main execution loop that pulls and runs jobs. This method runs in a dedicated
   * thread and continues until the worker is stopped.
   */
  void run();

 private:
  std::thread _thread;
  std::atomic<bool> _stop{false};
};

}  // namespace shkyera
