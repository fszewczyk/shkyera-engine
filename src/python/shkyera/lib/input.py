input_keys = set()


def add_key(key: str):
    """
    Add a key to the set of pressed keys.

    Parameters:
    key (str): The key to be added to the set.

    Returns:
    None
    """
    input_keys.add(key)


def is_pressed(key: str):
    """
    Check if a specific key is currently pressed.

    Parameters:
    key (str): The key code to be checked for being pressed. Check documentation for the list of available key codes.

    Returns:
    bool: True if the key is pressed, False otherwise.
    """
    return key in input_keys


def clear():
    """
    Clear the set of pressed keys.

    This function resets the set of pressed keys to an empty set, effectively clearing all previously pressed keys.

    Returns:
    None
    """
    global input_keys
    input_keys = set()
