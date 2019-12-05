from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager
from tag_reader.config import Config


# Create db
db = SQLAlchemy()

# Create instance of login manager
login_manager = LoginManager()
login_manager.login_view = 'users.login'
login_manager.login_message_category = 'info'

def create_app(config_class=Config):
    app = Flask(__name__)
    app.config.from_object(Config)

    db.init_app(app)
    login_manager.init_app(app)

    from tag_reader.main.views import main
    from tag_reader.users.views import users
    from tag_reader.errors.handlers import errors
    app.register_blueprint(main)
    app.register_blueprint(users)
    app.register_blueprint(errors)

    return app