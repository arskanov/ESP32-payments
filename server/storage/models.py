### Sqlalchemy Models

from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship
from sqlalchemy import Column, DateTime, ForeignKey, Integer, String, func, BigInteger

Base = declarative_base()

class Request(Base):
    __tablename__ = 'request'
    id = Column(Integer, primary_key=True)
    created_at = Column(DateTime, default=func.now())
    sign_attempts = Column(Integer, default=0)
    state = Column(String(250))


class Payment(Base):
    __tablename__ = 'payment'
    id = Column(Integer, ForeignKey('request.id'), primary_key=True)
    address = Column(String(250))
    created_at = Column(DateTime, default=func.now())



