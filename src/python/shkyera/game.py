from .logger import *


class Game():
    def __init__(self):
        pass

    def update(self):
        log_info(1)
        log_error(2)
        log_success(3, 2)
