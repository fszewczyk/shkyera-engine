/**
 * @file ObjectsWidget.hpp
 *
 * @brief Contains the declaration of the `ObjectsWidget` class, a user interface widget for managing game objects.
 */

#pragma once

#include <memory>
#include <vector>

#include <Game/Game.hpp>
#include <UI/Widget.hpp>

namespace shkyera {

/**
 * @brief A user interface widget for managing game objects.
 *
 * The `ObjectsWidget` class provides a graphical user interface for creating, listing, and selecting game objects.
 */
class ObjectsWidget : public Widget {
  public:
    using Widget::Widget;

    /**
     * @brief Set the game to associate with this widget.
     *
     * @param game A shared pointer to the game to associate with this widget.
     */
    void setGame(std::shared_ptr<Game> game);

    /**
     * @brief Implementation of the abstract `draw` method to render the game objects widget.
     */
    virtual void draw() override;

  private:
    /**
     * @brief Draw the creation panel for new game objects.
     */
    void drawCreate();

    /**
     * @brief Draw the list of game objects and allow selection.
     */
    void drawList();

    std::shared_ptr<Game> _game;                           ///< A shared pointer to the associated game.
};

} // namespace shkyera
