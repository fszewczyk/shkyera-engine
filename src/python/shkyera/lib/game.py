from .logger import *
from .drawer import *
from .physics import *


class Game():
    def __init__(self):
        self.objects = []

    def add_object(self, obj):
        self.objects.append(obj)

    def setup(self):
        for obj in self.objects:
            obj._setup()

    def update(self):
        draw_clear()

        for obj in self.objects:
            obj._update()
