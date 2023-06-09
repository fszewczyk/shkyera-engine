from .events import *


def log_info(*kwargs):
    message = " ".join([str(arg) for arg in kwargs])
    add_event(Event.LOG_INFO, message)


def log_error(*kwargs):
    message = " ".join([str(arg) for arg in kwargs])
    add_event(Event.LOG_ERROR, message)


def log_success(*kwargs):
    message = " ".join([str(arg) for arg in kwargs])
    add_event(Event.LOG_SUCCESS, message)


def log_verbose(*kwargs):
    message = " ".join([str(arg) for arg in kwargs])
    add_event(Event.LOG_VERBOSE, message)
