__author__="Phidgets Inc."
__date__="3-April-2019 12:00:00 PM"

from distutils.core import setup

setup (
	name = 'Phidget22',
	version = '1.1',
	description = 'Phidget22 Python wrapper library',
	auth = 'Phidgets Inc',
	author_email = 'support@phidgets.com',
	url = 'http://www.phidgets.com',
	packages = ['Phidget22', 'Phidget22.Devices'],
	license = 'GNU Library General Public License v2.0'
)
