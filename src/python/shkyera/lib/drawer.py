from .events import *


def draw_line(x1, y1, x2, y2):
    """
    Add a draw line event to the event queue.

    This function creates an event to draw a line from the point (x1, y1) to
    the point (x2, y2) on the canvas.

    Parameters:
        x1 (float): The x-coordinate of the starting point.
        y1 (float): The y-coordinate of the starting point.
        x2 (float): The x-coordinate of the ending point.
        y2 (float): The y-coordinate of the ending point.

    Returns:
        None
    """

    add_event(Event.DRAW_LINE, [x1, y1, x2, y2])


def draw_circle(x, y, r):
    """
    Add a draw circle event to the event queue.
    This function creates an event to draw a circle at (x, y) point with radius r.

    Args:
        x (float): The x-coordinate of the circle center
        y (flaot): The y-coordinate of the circle center
        r (float): The radius of the circle
    """
    add_event(Event.DRAW_CIRCLE, [x, y, r])


def draw_rectangle(x, y, w, h):
    """
    Add a draw rectangle event to the event queue.
    This function creates an event to draw a rectangle at (x, y) with size (w, h).

    Args:
        x (float): The x-coordinate of the top-left corner of the rectangle
        y (flaot): The y-coordinate of the top-left corner of the rectangle
        w (float): The width of the rectangle
        h (float): The height of the rectangle
    """
    add_event(Event.DRAW_RECTANGLE, [x, y, w, h])


def draw_clear():
    """
    Add a draw clear event to the event queue.

    This function creates an event to clear the rendered scene, removing all
    previously drawn lines and shapes.

    Parameters:
        None

    Returns:
        None
    """
    add_event(Event.DRAW_CLEAR, [])
