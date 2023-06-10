from .logger import *
from .drawer import *


class Game():
    def __init__(self):
        pass

    def update(self):
        log_info(1)
        log_error(2, 3, 1, "error", [1, 2])
        draw_circle(1.1, 2.3, 3.1, -1.2)
        draw_line(1, 2.3, -1.1, -1.2)
