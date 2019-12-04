from flask import render_template, Blueprint
from tag_reader import db
from tag_reader.models import Users
from tag_reader.users.forms import (RegistrationForm)

users = Blueprint('users', __name__)


@users.route("/register", methods=['GET', 'POST'])
def register():
    form = RegistrationForm()
    return render_template('register.html', title='Register', form=form)