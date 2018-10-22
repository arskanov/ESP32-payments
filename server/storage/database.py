from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, scoped_session

import logging

from server.storage.models import Base

_logger = logging.getLogger("logger")


class Database(object):
    def __init__(self, db_conf_block):
        self._type = db_conf_block['type']
        self._host = db_conf_block['host']
        self._port = db_conf_block['port']
        self._db = db_conf_block['database']
        self._username = db_conf_block['username']
        self._passwd = db_conf_block['password']

        self._initialized = False

        self._base = Base

        self._connect()
        if self._initialized == False:
            return

        self._base.metadata.bind = self._engine
        self._session = scoped_session(sessionmaker(bind=self._engine))
        self._session()

        if self._engine.dialect.has_table(self._engine, "payment") == False:
            _logger.info("Initializing database")
            self._base.metadata.create_all(self._engine)


    def _connect(self):
        if (self._type == 'postgresql'):
            try:

                self._engine = create_engine('postgresql://{}:{}@{}:{}/{}'.format(
                                             self._username, self._passwd, self._host, self._port, self._db))
                self._engine.connect()
                _logger.debug("Connection ok")
                self._initialized = True

            except:
                _logger.error("Connection failed")
                return

    def _disconnect(self):
        pass

    def commit(self):
        return self._session.commit()

    def query(self, query):
        return self._session.query(query)

    def ok(self):
        return self._initialized

    def add(self, item):
        self._session.add(item)

