from flask import render_template, url_for, flash, redirect, request, Blueprint
from flask_login import login_user, current_user, logout_user, login_required
from tag_reader import db, bcrypt
from sqlalchemy import desc
from tag_reader.models import People, Logs
from tag_reader.users.forms import (RegistrationForm, LoginForm, UpdateAccountForm,
                                    RequestResetForm, ResetPasswordForm)
from tag_reader.users.utils import send_reset_email, save_picture


users = Blueprint('users', __name__)


@users.route("/register", methods=['GET', 'POST'])
def register():
    if current_user.is_authenticated:
        return redirect(url_for('main.home'))
    form = RegistrationForm()
    if form.validate_on_submit():
        hashed_password = bcrypt.generate_password_hash(form.password.data).decode('utf-8')
        user = People(first_name=form.first_name.data,
                     last_name=form.last_name.data,
                     email=form.email.data,
                     password=hashed_password,
                     profile_pic='default.png'
                     )
        db.session.add(user)
        db.session.commit()
        flash(f'Your account has been created! You are now able to log in.', 'success')
        return redirect(url_for('users.login'))
    return render_template('register.html', title='Register', form=form)


@users.route("/login", methods=['GET', 'POST'])
def login():
    if current_user.is_authenticated:
        return redirect(url_for('main.home'))
    form = LoginForm()
    if form.validate_on_submit():
        user = People.query.filter_by(email=form.email.data).first()
        if user and bcrypt.check_password_hash(user.password, form.password.data):
            login_user(user, remember=form.remember.data)
            flash('Signed in!', 'success')
            return redirect(url_for('users.dashboard'))
        else:
            flash('Login Unsuccessful. Please check email and password', 'danger')
    return render_template('login.html', title='Login', form=form)


@users.route("/logout")
def logout():
    logout_user()
    flash('Signed out!', 'success')
    return redirect(url_for('main.home'))


@users.route("/dashboard", methods=['GET', 'POST'])
@login_required
def dashboard():
    form = UpdateAccountForm()
    if form.validate_on_submit():
        if form.profile_pic.data:
            picture_file = save_picture(form.profile_pic.data)
            current_user.profile_pic = picture_file
        current_user.email = form.email.data
        db.session.commit()
        flash('Your account has been updated!', 'success')
        return redirect(url_for('users.dashboard'))
    elif request.method == 'GET':
        form.first_name.data = current_user.first_name
        form.last_name.data = current_user.last_name
        form.email.data = current_user.email
        form.userid.data = current_user.userid
    profile_pic = url_for('static', filename='profile_pics/' + current_user.profile_pic)
    return render_template('dashboard.html',
                           title='Account',
                           profile_pic=profile_pic,
                           form=form)



# Show full attendance log
@users.route('/attendees')
@login_required
def attendees():
    ''' Function returns a list of all the scanned RFID tags by timestamp in
        descending order.
    '''
    attendees = People.query.join(Logs, People.userid==Logs.rfidtag).add_columns(People.first_name, People.last_name, Logs.inserted_date).order_by(desc(Logs.inserted_date))
    return render_template('attendance.html', attendees=attendees)


@users.route("/reset_password", methods=['GET', 'POST'])
def reset_request():
    if current_user.is_authenticated:
        return redirect(url_for('main.home'))
    form = RequestResetForm()
    if form.validate_on_submit():
        user = People.query.filter_by(email=form.email.data).first()
        send_reset_email(user)
        flash('An email has been sent with instructions to reset your password.', 'info')
        return redirect(url_for('users.login'))
    return render_template('reset_request.html', title='Reset Password', form=form)


@users.route("/reset_password/<token>", methods=['GET', 'POST'])
def reset_token(token):
    if current_user.is_authenticated:
        return redirect(url_for('main.home'))
    user = People.verify_reset_token(token)
    if user is None:
        flash('That is an invalid or expired token', 'warning')
        return redirect(url_for('users.reset_request'))
    form = ResetPasswordForm()
    if form.validate_on_submit():
        hashed_password = bcrypt.generate_password_hash(form.password.data).decode('utf-8')
        user.password = hashed_password
        db.session.commit()
        flash('Your password has been updated! You are now able to log in', 'success')
        return redirect(url_for('users.login'))
    return render_template('reset_token.html', title='Reset Password', form=form)