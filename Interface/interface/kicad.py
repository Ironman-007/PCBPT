
import math
import os

from flask import Blueprint, request, session
from kiutils.board import Board
from kiutils.schematic import Schematic
from werkzeug.utils import secure_filename

from interface import sio, UPLOAD_FOLDER

kicad_bp = Blueprint("kicad", __name__, url_prefix="/kicad")

SCHEMATICS_EXTENSION = ".kicad_sch"
LAYOUT_EXTENSION = ".kicad_pcb"
ALLOWED_FOOTPRINT_TAGS = ["capacitor", "resistor", "LED"]

schematics_data = {}
layouts_data = {}
nets_data = {}
candidates_data = {}

@sio.on("get_schematics_path")
def get_schematics_path():
    user_id = session.get("user_id")

    if user_id in schematics_data:
        sio.emit("schematics_path", schematics_data[user_id]["path"])

@sio.on("list_nets")
def list_nets():
    user_id = session.get("user_id")

    if user_id in nets_data:
        sio.emit("nets", nets_data[user_id])


@kicad_bp.route("/process_files", methods=["POST"])
def process_files():
    """
    Uploads the files to the server, saves them in the user's folder, and generates a net list.

    Returns:
        str: A message indicating that the files have been processed.
    """
    # Get the files from the request
    schematics_file = request.files["schematics_file"]
    layout_file = request.files["layout_file"]

    # Check if the file extensions are valid
    if not schematics_file.filename.endswith(SCHEMATICS_EXTENSION):
        return "Invalid schematics file extension"

    if not layout_file.filename.endswith(LAYOUT_EXTENSION):
        return "Invalid layout file extension"

    # Secure the filenames
    schematics_filename = secure_filename(schematics_file.filename)
    layout_filename = secure_filename(layout_file.filename)

    # Store the filenames in the session
    session["schematics_filename"] = schematics_filename
    session["layout_filename"] = layout_filename

    # Create the user's folder if it doesn't exist
    os.makedirs(os.path.join(UPLOAD_FOLDER, session["user_id"]), exist_ok=True)

    # Save the schematics file
    schematics_file_path = os.path.join(UPLOAD_FOLDER, session["user_id"], schematics_filename)
    schematics_file.save(schematics_file_path)

    # Emit a socket.io event with the path of the schematics file
    sio.emit("schematics_path", schematics_file_path)

    # Save the layout file
    layout_file.save(os.path.join(UPLOAD_FOLDER, session["user_id"], layout_filename))

    # Load the schematics and layout files
    schematics = Schematic().from_file(os.path.join(UPLOAD_FOLDER, session["user_id"], schematics_filename))
    board = Board().from_file(os.path.join(UPLOAD_FOLDER, session["user_id"], layout_filename))

    # Get the unique nets from the board
    nets = sorted(
        list(
            set(
                [
                    net.name if net.name[0] != "/" else net.name[1:]
                    for net in board.nets
                    if not net.name.startswith("unconnected") and net.name != ""
                ]
            )
        )
    )

    # Emit a socket.io event with the list of nets
    sio.emit("nets", nets)

    # Store the schematics, file path, layout, and nets in the data dictionaries
    schematics_data[session["user_id"]] = {
        "schematics": schematics,
        "path": schematics_file_path,
    }
    layouts_data[session["user_id"]] = board
    nets_data[session["user_id"]] = nets

    # Return a message indicating that the files have been processed
    return "Processed", 200


@kicad_bp.route("/select_nets", methods=["POST"])
def select_nets():
    """Emits the possible nets coordinates to the client"""

    first_net = request.form.get("first-net")
    second_net = request.form.get("second-net")

    if (
        session["user_id"] in layouts_data
        and session["user_id"] in nets_data
        and first_net in nets_data[session["user_id"]]
        and second_net in nets_data[session["user_id"]]
    ):
        user_id = session["user_id"]
        layouts = layouts_data[user_id]
        first_coordinates = []
        first_centers = []
        second_coordinates = []
        second_centers = []

        for footprint in layouts.footprints:
            if footprint.layer != "F.Cu":
                continue

            tag_allowed = False
            for tag in ALLOWED_FOOTPRINT_TAGS:
                if tag in footprint.tags:
                    tag_allowed = True
                    break

            if not tag_allowed:
                continue

            for pad in footprint.pads:
                if pad.type != "smd":
                    continue

                if pad.net is not None:
                    footprint_x = footprint.position.X
                    footprint_y = footprint.position.Y

                    if footprint.position.angle is not None:
                        footprint_angle = math.radians(footprint.position.angle)
                    elif pad.position.angle is not None:
                        footprint_angle = math.radians(pad.position.angle)
                    else:
                        footprint_angle = 0

                    if pad.net.name == first_net or pad.net.name == "/" + first_net:
                        pad_x = (
                            pad.position.X * math.cos(footprint_angle)
                            - pad.position.Y * math.sin(footprint_angle)
                            + footprint_x
                        )
                        pad_y = (
                            -pad.position.X * math.sin(footprint_angle)
                            - pad.position.Y * math.cos(footprint_angle)
                            + footprint_y
                        )

                        first_coordinates.append([pad_x, pad_y])
                        first_centers.append([footprint_x, footprint_y])

                    if pad.net.name == second_net or pad.net.name == "/" + second_net:
                        pad_x = (
                            pad.position.X * math.cos(footprint_angle)
                            - pad.position.Y * math.sin(footprint_angle)
                            + footprint_x
                        )
                        pad_y = (
                            pad.position.X * math.sin(footprint_angle)
                            - pad.position.Y * math.cos(footprint_angle)
                            + footprint_y
                        )

                        second_coordinates.append([pad_x, pad_y])
                        second_centers.append([footprint_x, footprint_y])

        if len(first_coordinates) > 0 and len(second_coordinates) > 0:
            x1 = [x[0] for x in first_coordinates]
            x2 = [x[0] for x in second_coordinates]
            delta1 = max(x1) - min(x2)
            delta2 = max(x2) - min(x1)
            if delta1 < delta2:
                left_index = x1.index(min(x1))
                right_index = x2.index(max(x2))
                left_coordinates = first_coordinates[left_index]
                left_center = first_centers[left_index]
                right_coordinates = second_coordinates[right_index]
                right_center = second_centers[right_index]
            else:
                left_index = x2.index(min(x2))
                right_index = x1.index(max(x1))
                left_coordinates = second_coordinates[left_index]
                left_center = second_centers[left_index]
                right_coordinates = first_coordinates[right_index]
                right_center = first_centers[right_index]
            candidates_data[user_id] = {
                "left": left_coordinates,
                "right": right_coordinates,
                "left_center": left_center,
                "right_center": right_center,
            }

            sio.emit(
                "net_coordinates",
                {
                    "coordinates": {first_net: first_coordinates, second_net: second_coordinates},
                    "candidates": candidates_data[user_id],
                },
            )

        else:
            sio.emit("net_coordinates", {"coordinates": {}, "candidates": {}})

    return "Selected"
