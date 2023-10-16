from typing import get_type_hints


def get_types(object_class):
    types = []
    for a in get_type_hints(object_class):
        types.append((a, get_type_hints(object_class)[a].__name__))
    return types
