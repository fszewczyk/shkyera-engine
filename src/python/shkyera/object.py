# Imports the boiler-plate object
from .lib.object import ShkyeraObject

from .lib.drawer import *


class Object(ShkyeraObject):
    def setup(self):
        # This part will be executed at the beginning of the program
        pass

    def update(self):
        # This part will be executed at each frame (approximately 60 times a second)
        draw_rectangle(
            x=50,
            y=10,
            w=400,
            h=50
        )
        draw_circle(120, 180, 60)
        draw_line(300, 200, 380, 260)
        pass
