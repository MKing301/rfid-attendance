"""This is the configuration file for the application."""
import os

class Config:
   SECRET_KEY = os.environ.get('SECRET_KEY_LOCAL')