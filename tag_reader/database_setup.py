import os
import datetime
from sqlalchemy import Column, ForeignKey, Integer, String,  DateTime
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship
from sqlalchemy import create_engine


Base = declarative_base()


class People(Base):
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

    __tablename__ = 'people'

    id = Column(Integer, primary_key=True)
    first_name = Column(String(250), nullable=False)
    last_name = Column(String(250), nullable=False)
    email = Column(String(250), nullable=False)
    password = Column(String(250), nullable=False)
    userid = Column(String(250), nullable=True, default=None)
    profile_pic = Column(String(250), nullable=False, default='default.png')
    created_date =  Column(DateTime, default=datetime.datetime.utcnow)


class Logs(Base):
    '''A log of scanned rfid tags with timestamp(utc):
       Attibute(s):
       id - unique id number
       rfidtag - rfid tag assigned to user
       inserted_date - timestamp of log entry in uct
    '''
    __tablename__ = 'logs'

    id = Column(Integer, primary_key=True)
    rfidtag = Column(String(250), nullable=True, default=None)
    inserted_date =  Column(DateTime, default=datetime.datetime.utcnow)
    user_id = Column(Integer, ForeignKey('people.id'))
    people = relationship(People)

engine = create_engine(os.environ.get('RFID_URI'))

Base.metadata.create_all(engine)
