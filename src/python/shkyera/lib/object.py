from .logger import *


class Vec3():
    x: float
    y: float
    z: float


class ShkyeraObject():
    def __init__(self, name, x, y, z, rx, ry, rz, sx, sy, sz):
        self._name = name

        self.position = Vec3()
        self.position.x = x
        self.position.y = y
        self.position.z = z

        self._rx = rx
        self._ry = ry
        self._rz = z

        self._sx = sx
        self._sy = sy
        self._sz = sz

    def _setup(self):
        try:
            self.setup()
        except Exception as e:
            log_error(f"Error: {e}")

    def setup(self):
        pass

    def _update(self):
        try:
            self.update()
        except Exception as e:
            log_error(f"Error: {e}")

    def update(self):
        pass

    def get_position(self):
        return self.position
