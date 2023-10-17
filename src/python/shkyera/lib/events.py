from enum import IntEnum


class Event(IntEnum):
    """
    Enumeration representing various event types.

    Enumerates different types of events that can be added to the event queue.
    """
    LOG_INFO = 0
    LOG_ERROR = 1
    LOG_SUCCESS = 2
    LOG_VERBOSE = 3

    DRAW_LINE = 4
    DRAW_CIRCLE = 5
    DRAW_RECTANGLE = 6
    DRAW_CLEAR = 7


events = []


def add_event(type: Event, payload: list):
    """
    Add an event to the event queue.

    This function appends a new event to the event queue.

    Parameters:
        type (Event): The type of the event, as defined in the Event enum.
        payload (list): A list containing additional data related to the event.

    Returns:
        None
    """
    events.append((int(type), payload))


def get():
    """
    Retrieve the list of events in the event queue.

    This function returns the current list of events in the event queue.

    Returns:
        list: A list of events, each represented as a tuple with (type, payload).
    """
    return events


def clear():
    """
    Clear the event queue.

    This function removes all events from the event queue, effectively emptying it.

    Returns:
        None
    """
    global events
    events = []
