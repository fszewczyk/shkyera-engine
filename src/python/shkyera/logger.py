_logs = {
    "info": [],
    "success": [],
    "verbose": [],
    "error": []
}


def clear():
    global _logs

    _logs = {
        "info": [],
        "success": [],
        "verbose": [],
        "error": []
    }


def log_info(*kwargs):
    message = " ".join([str(arg) for arg in kwargs])
    _logs["info"].append(message)


def log_error(*kwargs):
    message = " ".join([str(arg) for arg in kwargs])
    _logs["error"].append(message)


def log_success(*kwargs):
    message = " ".join([str(arg) for arg in kwargs])
    _logs["success"].append(message)


def log_verbose(*kwargs):
    message = " ".join([str(arg) for arg in kwargs])
    _logs["verbose"].append(message)


def get_info_logs():
    return _logs["info"]


def get_error_logs():
    return _logs["error"]


def get_success_logs():
    return _logs["success"]


def get_verbose_logs():
    return _logs["verbose"]
