class Entity():
    def __init__(self, uuid, x):
        self.uuid = uuid
        self.x = x

        self.__on_init()

    def __on_init(self):
        return

    def update(self):
        self.__on_update()

    def __on_update(self):
        self.x *= 2

    def get_status(self):
        return self.x
