from . import logger


def update():
    logger.log_info("Info")
    logger.log_error("Error")
    logger.log_success("Success")
    logger.log_verbose("Verbose")
