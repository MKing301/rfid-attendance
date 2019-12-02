from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from tag_reader.config import Config


# Create db
db = SQLAlchemy()

def create_app(config_class=Config):
    app = Flask(__name__)
    app.config.from_object(Config)

    db.init_app(app)

    from tag_reader.main.views import main
    from tag_reader.users.views import users
    app.register_blueprint(main)
    app.register_blueprint(user)

    return app