import os
import datetime
from sqlalchemy import Column, ForeignKey, Integer, String,  DateTime
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship
from sqlalchemy import create_engine


Base = declarative_base()


class Users(Base):
    '''A user in the RFID app program. Users will have
       the following attributes:
       Attibute(s):
       id - unique id number for each user
       first_name = user's first name
       last_name = user's last name
       email - user's email address
       password - user's password
       userid - user's associated rfid tag
       profile_pic - user's profile picture
       create_date - date user inserted into table
    '''

    __tablename__ = 'users'

    id = Column(Integer, primary_key=True)
    first_name = Column(String(250), nullable=False)
    last_name = Column(String(250), nullable=False)
    email = Column(String(250), nullable=False)
    password = Column(String(250), nullable=False)
    userid = Column(String(250), nullable=False)
    profile_pic = Column(String(250))
    create_date =  Column(DateTime, default=datetime.datetime.utcnow)


class Log(Base):
    '''A log of scanned rfid tags with timestamp(utc):
       Attibute(s):
       id - unique id number
       rfidtag - rfid tag assigned to user
       inserted_date - timestamp of log entry in uct
    '''
    __tablename__ = 'log'

    id = Column(Integer, primary_key=True)
    rfidtag = Column(String, ForeignKey('users.userid'))
    date_time =  Column(DateTime, default=datetime.datetime.utcnow)
    users = relationship(Users)


engine = create_engine(os.environ.get('RFID_URI'))

Base.metadata.create_all(engine)
