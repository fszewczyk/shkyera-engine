input_keys = set()


def add_key(key: str):
    input_keys.add(key)


def is_pressed(key: str):
    return key in input_keys


def clear():
    global input_keys
    input_keys = set()
