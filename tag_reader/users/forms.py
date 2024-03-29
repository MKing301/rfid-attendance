from flask_wtf import FlaskForm
from flask_wtf.file import FileField, FileAllowed
from wtforms import StringField, PasswordField, SubmitField, BooleanField
from wtforms.validators import DataRequired, Length, Email, EqualTo, ValidationError
from flask_login import current_user
from tag_reader.models import People


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

    def validate_email(self, email):
        user = People.query.filter_by(email=email.data).first()
        if user:
            raise ValidationError('That email is taken. Please choose a different one.')

class LoginForm(FlaskForm):
    email = StringField('Email',
                        validators=[DataRequired(),
                        Email()])
    password = PasswordField('Password',
                             validators=[DataRequired()])
    remember = BooleanField('Remember Me')
    submit = SubmitField('Login')


class UpdateAccountForm(FlaskForm):
    first_name = StringField('First Name', 
                            validators=[DataRequired(),Length(min=2, max=20)])
    last_name = StringField('Last Name', 
                            validators=[DataRequired(),Length(min=2, max=20)])
    email = StringField('Email',
                        validators=[DataRequired(), Email()])
    userid = StringField('UserID', 
                            validators=[DataRequired(),Length(min=2, max=20)])
    profile_pic = FileField('Update Profile Picture', validators=[FileAllowed(['jpg', 'png'])])
    submit = SubmitField('Update')

    def validate_email(self, email):
        if email.data != current_user.email:
            user = People.query.filter_by(email=email.data).first()
            if user:
                raise ValidationError('That email is taken. Please choose a different one.')


class RequestResetForm(FlaskForm):
    email = StringField('Email',
                        validators=[DataRequired(), Email()])
    submit = SubmitField('Request Password Reset')

    def validate_email(self, email):
        """ Validate email is valid in the database. """
        user = People.query.filter_by(email=email.data).first()
        if user is None:
            raise ValidationError('There is no account with that email.  You must register first.')


class ResetPasswordForm(FlaskForm):
    password = PasswordField('Password', validators=[DataRequired()])
    confirm_password = PasswordField('Confirm Password',
                                    validators=[DataRequired(),
                                    EqualTo('password')])
    submit = SubmitField('Reset Password')