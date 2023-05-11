class GameObject():
    def __init__(self):
        self.init()

    def init(self):
        self.on_init()

    def on_init(self):
        pass

    def update(self):
        self.on_update()

    def on_update(self):
        pass
