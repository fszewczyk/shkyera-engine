from .lib.object import ShkyeraObject

from .lib.drawer import *
from .lib.physics import *
from .lib.input import *


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
        if is_pressed('W'):
            self.position.y -= 0.02
        if is_pressed('S'):
            self.position.y += 0.02
        if is_pressed('A'):
            self.position.x -= 0.02
        if is_pressed('D'):
            self.position.x += 0.02

        self.rope.draw()
        self.rope.step()
