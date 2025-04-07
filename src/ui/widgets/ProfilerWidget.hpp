#pragma once

#include <GLFW/glfw3.h>
#include <Common/Profiler.hpp>
#include <UI/Widget.hpp>

namespace shkyera {

class ProfilerWidget : public Widget {
 public:
  using Widget::Widget;

  ProfilerWidget(std::string name);

  /**
     * @brief Implementation of the abstract `draw` method to render the profiler widget.
     */
  virtual void draw() override;

 private:
  void reset();

  bool mResetOnEachFrame = false;
  size_t mFramesSinceLastReset = 0;
  std::chrono::high_resolution_clock::time_point mTimeOfLastReset = std::chrono::high_resolution_clock::now();
};

}  // namespace shkyera
