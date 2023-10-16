from .events import *


def draw_line(x1, y1, x2, y2):
    add_event(Event.DRAW_LINE, [x1, y1, x2, y2])


def draw_clear():
    add_event(Event.DRAW_CLEAR, [])
