"""
Rename this file to config.py
"""



""" Database configuration
    Valid types are:
    * postgresql
"""

database = {
    "type": "postgresql",
    "database": "payment",
    "host": "localhost",
    "port": 5432,
    "username": "payment",
    "password": "payment"
}

""" Logging configuration """

log_file = "/var/log/payment.log"
log_level = "DEBUG"