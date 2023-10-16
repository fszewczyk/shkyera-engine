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
