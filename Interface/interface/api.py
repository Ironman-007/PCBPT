
import uuid

from flask import Blueprint, session

from interface import sio, UPLOAD_FOLDER

api_bp = Blueprint("api", __name__, url_prefix="/api")

@sio.on("connect")
def connect():
    """Checks if the client has a session"""
    # Generate a user ID if it doesn't exist in the session
    if "user_id" not in session:
        session["user_id"] = str(uuid.uuid4())
