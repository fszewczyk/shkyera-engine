/**
 * @file ObjectsWidget.hpp
 *
 * @brief Contains the declaration of the `ObjectsWidget` class, a user interface widget for managing game objects.
 */

#pragma once

#include <memory>
#include <vector>

#include "game/Game.hpp"
#include "ui/Widget.hpp"

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
     * @brief Get the currently selected game object.
     *
     * @return A shared pointer to the currently selected game object.
     */
    std::shared_ptr<GameObject> getSelectedObject() const;

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
    std::vector<std::shared_ptr<GameObject>> _gameObjects; ///< A list of game objects.
    size_t _selectedGameObject;                            ///< The index of the currently selected game object.
};

} // namespace shkyera
