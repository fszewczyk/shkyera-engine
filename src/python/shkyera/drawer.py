from .events import *


def draw_circle(x, y, radius, thickness):
    add_event(Event.DRAW_CIRCLE, [x, y, radius, thickness])


def draw_line(x1, y1, x2, y2):
    add_event(Event.DRAW_LINE, [x1, y1, x2, y2])
