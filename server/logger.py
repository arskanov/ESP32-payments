# Logger

import logging

import server.config as config

def set_logger(name):
    formatter = logging.Formatter(fmt='%(asctime)s - %(levelname)s - %(module)s - %(message)s')

    handler = logging.StreamHandler()
    handler.setFormatter(formatter)

    logger = logging.getLogger(name)
    level = logging.INFO

    if (config.log_level == "DEBUG"):
        level = logging.DEBUG

    logger.setLevel(logging.DEBUG)


    logger.addHandler(handler)

    filehandler = logging.FileHandler(config.log_file)
    logger.addHandler(filehandler)

    return logger
