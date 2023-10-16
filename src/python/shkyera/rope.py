from .lib.object import ShkyeraObject

from .lib.logger import *
from .lib.drawer import *
from .lib.physics import *


class Object(ShkyeraObject):
    Length: float
    Particles: int

    def setup(self):
        self.rope = Rope(
            origin=self.get_position(),
            alpha=0,
            length=self.Length,
            n_particles=self.Particles
        )

    def update(self):
        self.rope.draw()
        self.rope.step()
