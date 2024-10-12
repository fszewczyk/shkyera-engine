/**
 * @file PreviewWidget.hpp
 *
 * @brief Contains the declaration of the `PreviewWidget` class, a user interface widget for previewing content.
 */

#pragma once

#include <UI/Widget.hpp>

namespace shkyera {

/**
 * @brief A user interface widget for previewing content.
 *
 * The `PreviewWidget` class provides a graphical user interface widget for previewing various types of content,
 * which may include visual or interactive previews.
 */
class PreviewWidget : public Widget {
 public:
  using Widget::Widget;

  /**
     * @brief Implementation of the abstract `draw` method to render the preview widget.
     */
  virtual void draw() override;
};

}  // namespace shkyera
