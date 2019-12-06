"""This is the configuration file for the application."""
import os

class Config:
   SECRET_KEY = os.environ.get('SECRET_KEY_LOCAL')
   SQLALCHEMY_DATABASE_URI = os.environ.get('RFID_URI')