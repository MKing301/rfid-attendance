import os
import sys
import time
import datetime
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, String,  DateTime, ForeignKey
from sqlalchemy import create_engine
from sqlalchemy import create_engine, asc
from sqlalchemy.orm import scoped_session
from sqlalchemy.orm import sessionmaker
from sqlalchemy.orm import relationship
from selenium import webdriver
from Phidget22.Devices.RFID import *
from Phidget22.PhidgetException import *
from Phidget22.Phidget import *
from Phidget22.Net import *

# Returns a new base class from which all mapped classes should inherit
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
       userid - rfid tag associated with user
       profile_pic - user's profile picture
       created_date - date user inserted into table
    '''
    __tablename__ = 'people'

    id = Column(Integer, primary_key=True)
    first_name = Column(String(250), nullable=False)
    last_name = Column(String(250), nullable=False)
    email = Column(String(250), nullable=False)
    password = Column(String(250), nullable=False)
    userid = Column(String(250), nullable=False)
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
    rfidtag = Column(String, ForeignKey('people.userid'))
    inserted_date =  Column(DateTime, default=datetime.datetime.utcnow)  


engine = create_engine(os.environ.get('RFID_URI'))
Base.metadata.bind = engine
DBSession = sessionmaker(bind=engine)
session = scoped_session(DBSession)

try:
    ch = RFID()
except RuntimeError as e:
    print("Runtime Exception %s" % e.details)
    print("Press Enter to Exit...\n")
    readin = sys.stdin.read(1)
    exit(1)

def RFIDAttached(self):
    try:
        attached = self
        print("\nAttach Event Detected (Information Below)")
        print("===========================================")
        print("Library Version: %s" % attached.getLibraryVersion())
        print("Serial Number: %d" % attached.getDeviceSerialNumber())
        print("Channel: %d" % attached.getChannel())
        print("Channel Class: %s" % attached.getChannelClass())
        print("Channel Name: %s" % attached.getChannelName())
        print("Device ID: %d" % attached.getDeviceID())
        print("Device Version: %d" % attached.getDeviceVersion())
        print("Device Name: %s" % attached.getDeviceName())
        print("Device Class: %d" % attached.getDeviceClass())
        print("\n")

    except PhidgetException as e:
        print("Phidget Exception %i: %s" % (e.code, e.details))
        print("Press Enter to Exit...\n")
        readin = sys.stdin.read(1)
        exit(1)   
    
def RFIDDetached(self):
    detached = self
    try:
        print("\nDetach event on Port %d Channel %d" % (detached.getHubPort(), detached.getChannel()))
    except PhidgetException as e:
        print("Phidget Exception %i: %s" % (e.code, e.details))
        print("Press Enter to Exit...\n")
        readin = sys.stdin.read(1)
        exit(1)   

def ErrorEvent(self, eCode, description):
    print("Error %i : %s" % (eCode, description))

def TagHandler(self, tag, protocol):


    # Create record for scanned tag to insert into log table
    log = Logs(rfidtag=tag)

    # Stage record to insert into table
    session.add(log)

    # Commit record
    session.commit()


    # Query database to obtain name associated with scanned tag
    data = session.query(People).filter_by(userid=tag).first()


    # Close session
    session.close()
    
    # Open welcome template and insert user's profile picture and first & last name into the body
    try:
        person = open('/home/mfsd1809/Dev/FullStackWebDeveloper/GitRepos/rfid-attendance/tag_reader/templates/welcome.html', 'w')

        fname = data.first_name
        lname = data.last_name
        pic = f"/home/mfsd1809/Dev/FullStackWebDeveloper/GitRepos/rfid-attendance/tag_reader/static/profile_pics/{data.profile_pic}"

        message = f"""
        <html>
            <head>
                <title>Person</title>
            </head>
            <body>
                <img src="{pic}" alt="image" height="400" width="400">
                <p style="font-size:60px;">{fname} {lname}</p>
            </body>
        </html>
        """
        person.write(message)
        person.close()

        # Use Selenium to open browser, display user's profile image and first & last name for 3 seconds using welcome template
        browser = webdriver.Firefox(executable_path='/home/mfsd1809/Dev/FullStackWebDeveloper/GitRepos/rfid-attendance/tag_reader/geckodriver')
        browser.get('file:////home/mfsd1809/Dev/FullStackWebDeveloper/GitRepos/rfid-attendance/tag_reader/templates/welcome.html')
        time.sleep(3)
        browser.close()
        browser.quit()

    except Exception as e:
        print(f'Exception: {e}')

try:
    ch.setOnAttachHandler(RFIDAttached)
    ch.setOnDetachHandler(RFIDDetached)
    ch.setOnErrorHandler(ErrorEvent)

    ch.setOnTagHandler(TagHandler)
    #ch.setOnTagLostHandler(TagLostHandler)

    # Please review the Phidget22 channel matching documentation for details on the device
    # and class architecture of Phidget22, and how channels are matched to device features.

    # Specifies the serial number of the device to attach to.
    # For VINT devices, this is the hub serial number.
    #
    # The default is any device.
    #
    # ch.setDeviceSerialNumber(<YOUR DEVICE SERIAL NUMBER>) 

    # For VINT devices, this specifies the port the VINT device must be plugged into.
    #
    # The default is any port.
    #
    # ch.setHubPort(0)

    # Specifies which channel to attach to.  It is important that the channel of
    # the device is the same class as the channel that is being opened.
    #
    # The default is any channel.
    #
    # ch.setChannel(0)

    # In order to attach to a network Phidget, the program must connect to a Phidget22 Network Server.
    # In a normal environment this can be done automatically by enabling server discovery, which
    # will cause the client to discovery and connect to available servers.
    #
    # To force the channel to only match a network Phidget, set remote to 1.
    #
    # Net.enableServerDiscovery(PhidgetServerType.PHIDGETSERVER_DEVICEREMOTE);
    # ch.setIsRemote(1)

    print("Waiting for the Phidget RFID Object to be attached...")
    ch.openWaitForAttachment(5000)
except PhidgetException as e:
    print("Phidget Exception %i: %s" % (e.code, e.details))
    print("Press Enter to Exit...\n")
    readin = sys.stdin.read(1)
    exit(1)

print("Gathering data for 30 seconds...")
time.sleep(30)

try:
    ch.close()
except PhidgetException as e:
    print("Phidget Exception %i: %s" % (e.code, e.details))
    print("Press Enter to Exit...\n")
    readin = sys.stdin.read(1)
    exit(1) 
print("Closed RFID device")
exit(0)
