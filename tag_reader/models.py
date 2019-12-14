from itsdangerous import TimedJSONWebSignatureSerializer as Serializer
from sqlalchemy import Column, ForeignKey, Integer, String,  DateTime
from flask import current_app
from tag_reader import db, login_manager
from flask_login import UserMixin
import datetime


@login_manager.user_loader
def load_user(user_id):
    return People.query.get(int(user_id))

# Model for people table in database for users that register.
class People(db.Model, UserMixin):
    id = db.Column(db.Integer, primary_key=True)
    first_name = db.Column(db.String(120), unique=True, nullable=False)
    last_name = db.Column(db.String(120), unique=True, nullable=False)
    email = db.Column(db.String(255), unique=True, nullable=False)
    password = db.Column(db.String(255), nullable=False)
    userid = db.Column(db.String(50), unique=True, nullable=False)
    profile_pic = db.Column(db.String(255), nullable=False, default='default.jpg')
    created_date =  db.Column(DateTime, default=datetime.datetime.utcnow)


    def get_reset_token(self, expires_sec=1800):
        """ Get a token for current user to reset password.  The token
            expires after 30 minutes.
        """
        s = Serializer(current_app.config['SECRET_KEY'], expires_sec)
        return s.dumps({'user_id':self.id}).decode('utf-8')


    @staticmethod
    def verify_reset_token(token):
        """ Verify the token is valid for the current user. """
        s = Serializer(current_app.config['SECRET_KEY'])
        try:
            user_id = s.loads(token)['user_id']
        except:
            return None
        return People.query.get(user_id)


    def __repr__(self):
        return f"People('{self.fist_name}, {self.last_name}, {self.email}')"

# Model for log table in database to capture scanned rfid tags.
class Logs(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    rfidtag = db.Column(db.String(255), nullable=False)
    inserted_date = db.Column(db.DateTime, nullable=False, default=datetime.datetime.utcnow)