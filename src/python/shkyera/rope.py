from .lib.object import ShkyeraObject

from .lib.logger import *
from .lib.drawer import *
from .lib.physics import *


class Object(ShkyeraObject):
    Length: float
    Particles: int

    def setup(self):
        self.strings = [String(self.get_position(), 0,
                               self.Length, self.Particles)]

    def update(self):
        for s in self.strings:
            s.draw()
            s.step()
