from sqlalchemy import Column, ForeignKey, Integer, String,  DateTime
from flask import current_app
from tag_reader import db, login_manager
from flask_login import UserMixin
import datetime


@login_manager.user_loader
def load_user(user_id):
    return People.query.get(int(user_id))

class People(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key=True)
    first_name = db.Column(db.String(120), unique=True, nullable=False)
    last_name = db.Column(db.String(120), unique=True, nullable=False)
    email = db.Column(db.String(255), unique=True, nullable=False)
    password = db.Column(db.String(255), nullable=False)
    userid = db.Column(db.String(50), unique=True, nullable=False)
    profile_pic = db.Column(db.String(255), nullable=False, default='default.jpg')
    created_date =  db.Column(DateTime, default=datetime.datetime.utcnow)


class Logs(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    rfidtag = db.Column(db.String(255), nullable=False)
    inserted_date = db.Column(db.DateTime, nullable=False, default=datetime.datetime.utcnow)