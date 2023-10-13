from .logger import *
from .drawer import *

import random


class Game():
    def __init__(self):
        pass

    def update(self):
        draw_line(random.randint(20, 80), random.randint(20, 80), 100, 100)
        log_info("Sample info!!!")
        log_error("Sample error")
