from sqlalchemy import Column, ForeignKey, Integer, String,  DateTime
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship
from sqlalchemy import create_engine
import datetime

Base = declarative_base()


class Users(Base):
    '''A user of the catalog app program. Users will have
       the following attributes:
       Attibute(s):
       id - unique id number for each user
       first_name = user's first name
       last_name = user's last name
       email - user's email address
       password - user's passwoclass Book(Base):
    '''

    __tablename__ = 'users'

    id = Column(Integer, primary_key=True)
    first_name = Column(String(250), nullable=False)
    last_name = Column(String(250), nullable=False)
    email = Column(String(250), nullable=False)
    password = Column(String(250), nullable=False)
    userid = Column(String(250), nullable=False)
    profile_pic = Column(String(250))
    inserted_date =  Column(DateTime, default=datetime.datetime.utcnow)


class Log(Base):
    '''A category for the books in the catalog app program. Categories
       will have the following attributes:
       Attibute(s):
       id - unique id number
       user_id - id of user that created the category
    '''
    __tablename__ = 'log'

    id = Column(Integer, primary_key=True)
    rfidtag = Column(String, ForeignKey('users.userid'))
    inserted_date =  Column(DateTime, default=datetime.datetime.utcnow)
    users = relationship(Users)


engine = create_engine('postgresql://postgres:postgres@localhost:5432/attendence')

Base.metadata.create_all(engine)
