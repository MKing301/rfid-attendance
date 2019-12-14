import os
import secrets
from PIL import Image
from flask import url_for, current_app
from flask_mail import Message
from tag_reader import mail


def send_reset_email(user):
    """ Send an email to provided address with instructions on how
        to reset the password for the user associated with the email.'
    """
    token = user.get_reset_token()
    msg = Message('Password Reset Request', 
                   sender=os.environ.get('MAIL_USERNAME'),
                   recipients=[user.email])
    msg.body = '''To reset your password, visit the following link:
{0}

If you did not make this request, simply ignore this email and no change will be made.
'''.format({url_for('users.reset_token', token=token, _external=True)})
    mail.send(msg)


def save_picture(form_picture):
    """ Allow current user to upload a profile picture."""
    random_hex = secrets.token_hex(8)
    _, f_ext = os.path.splitext(form_picture.filename)
    picture_fn = random_hex + f_ext
    picture_path = os.path.join(current_app.root_path, 'static/profile_pics', picture_fn)
    output_size = (125, 125)
    i = Image.open(form_picture)
    i.thumbnail(output_size)
    i.save(picture_path)
    return picture_fn
