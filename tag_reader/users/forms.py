from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, SubmitField, BooleanField
from wtforms.validators import DataRequired, Length, Email, EqualTo, ValidationError
from flask_login import current_user
from tag_reader.models import Users


class RegistrationForm(FlaskForm):
    first_name = StringField('First Name', 
                            validators=[DataRequired(),Length(min=2, max=25)])
    last_name = StringField('Last Name', 
                            validators=[DataRequired(),Length(min=2, max=25)])
    email = StringField('Email',
                        validators=[DataRequired(), Email()])
    password = PasswordField('Password', validators=[DataRequired()])
    confirm_password = PasswordField('Confirm Password',
                                    validators=[DataRequired(),
                                    EqualTo('password')])
    submit = SubmitField('Register')