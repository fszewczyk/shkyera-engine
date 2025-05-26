#pragma once

#include <thread>

namespace shkyera {

class ThreadWorker {
 public:
  ThreadWorker();
  ~ThreadWorker();

  void run();

 private:
  std::thread _thread;
  std::atomic<bool> _stop{false};
};

}  // namespace shkyera
