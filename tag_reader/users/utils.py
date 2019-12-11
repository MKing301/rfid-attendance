import os
from flask import url_for, current_app
from flask_mail import Message
from tag_reader import mail


def send_reset_email(user):
    token = user.get_reset_token()
    msg = Message('Password Reset Request', 
                   sender=os.environ.get('MAIL_USERNAME'),
                   recipients=[user.email])
    msg.body = '''To reset your password, visit the following link:
{0}

If you did not make this request, simply ignore this email and no change will be made.
'''.format({url_for('users.reset_token', token=token, _external=True)})
    mail.send(msg)