import math
import os
import uuid

import serial
from kiutils.board import Board
from kiutils.schematic import Schematic
from flask import Blueprint, request, session
from serial.tools import list_ports
from werkzeug.utils import secure_filename

from interface import sio, UPLOAD_FOLDER

api_bp = Blueprint("api", __name__, url_prefix="/api")

SCHEMATICS_EXTENSION = ".kicad_sch"
LAYOUT_EXTENSION = ".kicad_pcb"
ALLOWED_FOOTPRINT_TAGS = ["capacitor", "resistor", "LED"]

serial_connections = {}
schematics_data = {}
layouts_data = {}
nets_data = {}
candidates_data = {}

@api_bp.route("/connect_device", methods=["POST"])
def connect_device():
    """Connects to the serial port"""
    port = request.form.get("port")
    baudrate = request.form.get("baudrate")

    if port is not None:
        if port not in serial_connections:
            serial_connections[port] = serial.Serial(port, baudrate)
        elif not serial_connections[port].is_open:
            serial_connections[port].open()

        session["port"] = port
        session["baudrate"] = baudrate

    sio.emit("device_connected", True)

    return "Connected"


@api_bp.route("/disconnect_device", methods=["POST"])
def disconnect_device():
    """Disconnects from the serial port"""
    if "port" in session:
        serial_connections[session["port"]].close()
        del serial_connections[session["port"]]
        del session["port"]
        del session["baudrate"]

    sio.emit("device_connected", False)

    return "Disconnected"


@sio.on("list_serial_devices")
def list_serial_devices():
    """Emits a list of available serial ports to the client"""
    ports = [port.device for port in list_ports.comports()]
    existing_ports = list(serial_connections.keys())
    for port in existing_ports:
        if port not in ports:
            del serial_connections[port]
    connected = []
    for port in ports:
        if port in serial_connections:
            connected.append(True)
        else:
            connected.append(False)

    devices = [{"port": port, "connected": connected} for port, connected in zip(ports, connected)]
    sio.emit("serial_devices", devices)



@api_bp.route("/send_command", methods=["POST"])
def send_command():
    """Sends a command to the serial port"""
    command = request.form.get("command")

    if "port" in session:
        serial_connections[session["port"]].write(command.encode())
    else:
        sio.emit("command_response", f"Command error '{command}': Device not connected")
    return "Sent"

@api_bp.route("/upload_command", methods=["POST"])
def upload_command():
    """Upload a file containing commands to be sent to the serial port"""
    if "port" in session:
        if "command_file" in request.files:
            file = request.files["command_file"]
            file.save("commands.pcbpt")
            with open("commands.pcbpt", "r") as f:
                for line in f:
                    # serial_connections[session["port"]].write(line.encode())
                    sio.emit("command_response", line)
        else:
            sio.emit("command_response", "File Upload Error: No file provided")
    else:
        sio.emit("command_response", "File Upload Error: Device not connected")
    return "Sent"

@api_bp.route("/process_files", methods=["POST"])
def process_files():
    """Uploads the files to the server, saves them in the user's folder and generate a net list"""
    # thickness = request.form.get("thickness")
    schematics_file = request.files["schematics_file"]
    layout_file = request.files["layout_file"]

    if not schematics_file.filename.endswith(SCHEMATICS_EXTENSION):
        return "Invalid schematics file extension"

    if not layout_file.filename.endswith(LAYOUT_EXTENSION):
        return "Invalid layout file extension"

    schematics_filename = secure_filename(schematics_file.filename)
    layout_filename = secure_filename(layout_file.filename)

    session["schematics_filename"] = schematics_filename
    session["layout_filename"] = layout_filename
    # session["thickness"] = thickness

    if "user_id" not in session:
        session["user_id"] = str(uuid.uuid4())

    os.makedirs(os.path.join(UPLOAD_FOLDER, session["user_id"]), exist_ok=True)
    schematics_file_path = os.path.join(UPLOAD_FOLDER, session["user_id"], schematics_filename)
    schematics_file.save(schematics_file_path)
    sio.emit("schematics_file", schematics_file_path)
    layout_file.save(os.path.join(UPLOAD_FOLDER, session["user_id"], layout_filename))

    schematics = Schematic().from_file(os.path.join(UPLOAD_FOLDER, session["user_id"], schematics_filename))
    board = Board().from_file(os.path.join(UPLOAD_FOLDER, session["user_id"], layout_filename))

    nets = sorted(list(set([net.name if net.name[0] != "/" else net.name[1:] for net in board.nets if not net.name.startswith("unconnected") and net.name != ""])))

    sio.emit("nets", nets)

    schematics_data[session["user_id"]] = schematics
    layouts_data[session["user_id"]] = board
    nets_data[session["user_id"]] = nets

    return "Processed"


@api_bp.route("/select_nets", methods=["POST"])
def select_nets():
    """Emits the possible nets coordinates to the client"""

    first_net = request.form.get("first-net")
    second_net = request.form.get("second-net")

    if ("user_id" in session and session["user_id"] in layouts_data and session["user_id"] in nets_data and
        first_net in nets_data[session["user_id"]] and second_net in nets_data[session["user_id"]]):
        layouts = layouts_data[session["user_id"]]
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

                    if pad.net.name == first_net or pad.net.name == "/"+first_net:
                        pad_x = pad.position.X * math.cos(footprint_angle) - pad.position.Y * math.sin(footprint_angle) + footprint_x
                        pad_y = -pad.position.X * math.sin(footprint_angle) - pad.position.Y * math.cos(footprint_angle) + footprint_y

                        first_coordinates.append([pad_x, pad_y])
                        first_centers.append([footprint_x, footprint_y])

                    if pad.net.name == second_net or pad.net.name == "/"+second_net:
                        pad_x = pad.position.X * math.cos(footprint_angle) - pad.position.Y * math.sin(footprint_angle) + footprint_x
                        pad_y = pad.position.X * math.sin(footprint_angle) - pad.position.Y * math.cos(footprint_angle) + footprint_y

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
            candidates_data[session["user_id"]] = {"left": left_coordinates, "right": right_coordinates, "left_center": left_center, "right_center": right_center}

            sio.emit("net_coordinates", {"coordinates": {first_net: first_coordinates, second_net: second_coordinates}, "candidates": candidates_data[session["user_id"]]})

        else:
            sio.emit("net_coordinates", {"coordinates": {}, "candidates": {}})

    return "Selected"
