import os
import uuid

from flask import Flask, send_from_directory, render_template, g, session
from flask_cors import CORS
from flask_socketio import SocketIO

UPLOAD_FOLDER = "interface/static/uploads/"
sio = SocketIO(
        logger=False,
        engineio_logger=False,
        cors_allowed_origins="*",
        path=f"/socket.io",
    )


def generate_id():
    """Generate a user ID if it doesn't exist in the session"""
    if "user_id" not in session:
        # print("Generating user ID")
        session["user_id"] = str(uuid.uuid4())

def create_app(test_config=None):
    app = Flask(__name__, instance_relative_config=True)
    app.config.from_mapping(
        SECRET_KEY="dev",
        DATABASE=os.path.join(app.instance_path, "interface.sqlite"),
    )
    app.config["UPLOAD_FOLDER"] = UPLOAD_FOLDER

    if test_config is None:
        app.config.from_pyfile('config.py', silent=True)
    else:
        app.config.from_mapping(test_config)

    # ensure the instance folder exists
    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    CORS(app)

    @app.route("/static/<path:path>")
    def global_static(path):
        return send_from_directory("static", path)

    @app.route("/")
    def home():
        generate_id()
        return render_template("base.html")

    from interface.serial import serial_bp
    app.register_blueprint(serial_bp)

    from interface.kicad import kicad_bp
    app.register_blueprint(kicad_bp)

    sio.init_app(app)

    return app

app = create_app()

if __name__ == "__main__":
    sio.run(app, host="0.0.0.0", port=5000)
