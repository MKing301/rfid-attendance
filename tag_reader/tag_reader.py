import sys
import time
import datetime
import psycopg2
import psycopg2.extras
from selenium import webdriver
from Phidget22.Devices.RFID import *
from Phidget22.PhidgetException import *
from Phidget22.Phidget import *
from Phidget22.Net import *


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

    # Get a connection

    conn = psycopg2.connect(database='attendance', user='postgres', password='postgres', host='localhost')

    # conn.cursor will return a cursor object, you can use this cursor to perform queries
    cur = conn.cursor()

    # execute query
    cur.execute('INSERT INTO log (rfidtag) VALUES (%s)', [tag])

    #commit record(s)
    conn.commit()

    # Close the current cursor
    cur.close()

    # dict cursor which allows access to the retrieved
    # records using an interface similar to the Python dictionaries to perform
    # queries
    dict_cur = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    dict_cur.execute("SELECT * FROM users WHERE userid = %s", [tag])
    # retrieve one record from the database
    data = dict_cur.fetchone()

    # close dictionary cursor
    dict_cur.close()

    # Close the communication with the PostgreSQL database
    conn.close()
    
    try:
        person = open('/home/mfsd1809/Dev/FullStackWebDeveloper/GitRepos/rfid-attendance/tag_reader/templates/welcome.html', 'w')

        fname = data['first_name']
        lname = data['last_name']
        pic = f"/home/mfsd1809/Dev/FullStackWebDeveloper/GitRepos/rfid-attendance/tag_reader/static/images/{data['profile_pic']}"

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

        # Selenium
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
