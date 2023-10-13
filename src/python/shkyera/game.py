from .logger import *
from .drawer import *

from .physics import *


class Game():
    def __init__(self):
        pass
        self.strings = [String(i+0.5, 1.0, i, 0.3 + 0.1 * i, 7)
                        for i in range(4)]

    def update(self):
        pass
        draw_clear()

        for j, s in enumerate(self.strings):
            particles = s.getParticles()
            for i in range(1, len(particles)):
                draw_line(200 * particles[i-1][0], 200 * particles[i-1][1],
                          200 * particles[i][0], 200 * particles[i][1])

            self.strings[j].step()
