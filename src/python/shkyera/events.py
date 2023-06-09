from enum import IntEnum


class Event(IntEnum):
    LOG_INFO = 0
    LOG_ERROR = 1
    LOG_SUCCESS = 2
    LOG_VERBOSE = 3

    COMPONENT_TRANSFORM = 4
    COMPONENT_SCRIPT = 5
    COMPONENT_RIGIDBODY = 6


events = []


def add_event(type: Event, payload: list):
    events.append((int(type), payload))


def get():
    return events


def clear():
    events = []
