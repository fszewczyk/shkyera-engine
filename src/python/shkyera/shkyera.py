from . import logger


def update():
    try:
        logger.log_success("Success", 6, 9, 12, [1, 2, 3], {'au': 3})
    except Exception as error:
        logger.log_error("Error occured:", error)
