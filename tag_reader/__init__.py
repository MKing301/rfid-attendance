from flask import Flask
from tag_reader.config import Config


def create_app(config_class=Config):
    app = Flask(__name__)
    app.config.from_object(Config)

    from tag_reader.main.views import main
    app.register_blueprint(main)

    return app