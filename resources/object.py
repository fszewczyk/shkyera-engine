# Imports the boiler-plate object
from .lib.object import ShkyeraObject

from .lib.drawer import *


class Object(ShkyeraObject):
    def setup(self):
        # This part will be executed at the beginning of the program
        pass

    def update(self):
        draw_circle(300, 250, 150)
        # This part will be executed at each frame (approximately 60 times a second)
        pass
