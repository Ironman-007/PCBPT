
import math
import os
from typing import List, Dict

import kiutils
from flask import Blueprint, request, session
from kiutils.board import Board
from kiutils.schematic import Schematic
from werkzeug.utils import secure_filename

from interface import sio, UPLOAD_FOLDER, generate_id

kicad_bp = Blueprint("kicad", __name__, url_prefix="/kicad")

SCHEMATICS_EXTENSION = ".kicad_sch"
LAYOUT_EXTENSION = ".kicad_pcb"
ALLOWED_FOOTPRINT_TAGS = ["capacitor", "resistor", "LED"]

MIN_PAD_SIZE = 1.5
BOARD_ORIENTATION = 0

schematics_data = {}
layouts_data = {}
nets_data = {}
candidates_data = {}
edges_data = {}
pads_data = {}
dimensions_data = {}

@sio.on("get_schematics_path")
def get_schematics_path():
    """Emits the path of the schematics file stored in the current session to the client"""
    user_id = session.get("user_id")

    if user_id in schematics_data:
        sio.emit("schematics_path", schematics_data[user_id]["path"])

@sio.on("list_nets")
def list_nets():
    """Emits the list of nets stored in the current session to the client"""
    user_id = session.get("user_id")

    if user_id in nets_data:
        sio.emit("nets", nets_data[user_id])


@sio.on("list_edges")
def list_edges():
    """Emits the list of edges stored in the current session to the client"""
    user_id = session.get("user_id")

    if user_id in edges_data:
        sio.emit("edges", edges_data[user_id])


@sio.on("list_pads")
def list_pads():
    """Emits the list of pads stored in the current session to the client"""
    user_id = session.get("user_id")

    if user_id in pads_data:
        sio.emit("pads", pads_data[user_id])

@sio.on("list_dimensions")
def list_dimensions():
    """Emits the list of dimensions stored in the current session to the client"""
    user_id = session.get("user_id")

    if user_id in dimensions_data:
        sio.emit("dimensions", dimensions_data[user_id])

@kicad_bp.route("/process_files", methods=["POST"])
def process_files():
    """
    Uploads the files to the server, saves them in the user's folder, and generates a net list.

    Returns:
        str: A message indicating that the files have been processed.
    """

    generate_id()
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
    edges, origin, width, height = get_edges(board)
    pads = get_pads(board, origin)

    # Emit a socket.io event with the list of nets
    sio.emit("nets", nets)

    # Store the schematics, file path, layout, and nets in the data dictionaries
    schematics_data[session["user_id"]] = {
        "schematics": schematics,
        "path": schematics_file_path,
    }
    layouts_data[session["user_id"]] = board
    nets_data[session["user_id"]] = nets
    edges_data[session["user_id"]] = edges
    pads_data[session["user_id"]] = pads
    dimensions_data[session["user_id"]] = {"origin": origin, "width": width, "height": height}

    sio.emit("edges", edges)
    sio.emit("pads", pads)
    sio.emit("dimensions", {"origin": origin, "width": width, "height": height})

    # Return a message indicating that the files have been processed
    return "Processed", 200


def get_edges(layout: Board) -> List[Dict]:
    """Returns the edges of the layout"""
    edges = [e for e in layout.graphicItems if e.layer == "Edge.Cuts"]

    json_edges = []
    for e in edges:
        if type(e) == type(kiutils.items.gritems.GrPoly()):
            for i in range(len(e.coordinates) - 1):
                p = e.coordinates[i]
                p2 = e.coordinates[i + 1]
                json_edges.append(
                    {
                        "start": {"x": p.X, "y": p.Y, "angle": p.angle if p.angle != None else 0},
                        "end": {"x": p2.X, "y": p2.Y, "angle": p2.angle if p2.angle != None else 0},
                    }
                )
            p = e.coordinates[-1]
            p2 = e.coordinates[0]
            json_edges.append(
                {
                    "start": {"x": p.X, "y": p.Y, "angle": p.angle if p.angle != None else 0},
                    "end": {"x": p2.X, "y": p2.Y, "angle": p2.angle if p2.angle != None else 0},
                }
            )
        if type(e) == type(kiutils.items.gritems.GrLine()):
            json_edges.append({
                "start": {"x": e.start.X, "y": e.start.Y, "angle": e.start.angle if e.start.angle != None else 0},
                "end": {"x": e.end.X, "y": e.end.Y, "angle": e.end.angle if e.end.angle != None else 0}
                }
            )

    origin = [min([e["start"]["x"] for e in json_edges]), min([e["start"]["y"] for e in json_edges])]
    width = max([e["start"]["x"] for e in json_edges]) - origin[0]
    height = max([e["start"]["y"] for e in json_edges]) - origin[1]

    # Sort edges by matching start and end points
    new_edges = []

    current_edge = json_edges.pop(0)
    while len(json_edges) > 0:
        for i, edge in enumerate(json_edges):
            if edge["start"] == current_edge["end"]:
                new_edges.append(current_edge)
                current_edge = json_edges.pop(i)
                break
            elif edge["end"] == current_edge["end"]:
                edge["start"], edge["end"] = edge["end"], edge["start"]
                new_edges.append(current_edge)
                current_edge = json_edges.pop(i)
                break
        else:
            new_edges.append(current_edge)
            current_edge = json_edges.pop(0)
    new_edges.append(current_edge)

    # Convert coordinates to relative coordinates
    for edge in new_edges:
        edge["start"]["x"] -= origin[0]
        edge["start"]["y"] -= origin[1]
        edge["end"]["x"] -= origin[0]
        edge["end"]["y"] -= origin[1]

    return new_edges, origin, width, height


def get_pads(layout: Board, origin: List):
    """Returns the pads of the layout"""

    footprints = [f for f in layout.footprints if f.layer == "F.Cu"]

    grav_centers = [[
        sum(gcs_x := [p.position.X for p in f.pads]) / len(gcs_x),
        sum(gcs_y := [p.position.Y for p in f.pads]) / len(gcs_y)
    ] for f in footprints]

    pads = [
        [
            {
                "id": p.tstamp,
                "footprint_angle": f.position.angle if f.position.angle is not None else 0,
                "footprint_x": f.position.X - origin[0],
                "footprint_y": f.position.Y - origin[1],
                "pad_angle": p.position.angle if p.position.angle is not None else 0,
                "pad_x": p.position.X,
                "pad_y": p.position.Y,
                "size_x": p.size.X,
                "size_y": p.size.Y,
                "grav_center_x": gc[0],
                "grav_center_y": gc[1],
                "relative_angle": math.atan2(p.position.Y - gc[1], p.position.X - gc[0]),
                "size_angle": p.size.angle if p.size.angle is not None else 0,
                "net": (p.net.name if p.net.name[0] != "/" else p.net.name[1:]) if p.net is not None else "",
                "shape": p.shape,
                "roundrect_ratio": p.roundrectRatio if p.roundrectRatio is not None else 0,
                "is_allowed":is_pad_valid(p),
                "tags": f.tags if f.tags is not None else [],
            } for p in f.pads
        ] for f, gc in zip(footprints, grav_centers)
    ]

    return pads


def is_pad_valid(pad) -> bool:
    """Returns whether the pad is valid or not"""
    return pad.type == "smd" and pad.net != "" and pad.size.X * pad.size.Y >= MIN_PAD_SIZE


@kicad_bp.route("/select_nets", methods=["POST"])
def select_nets():
    """Emits the possible nets coordinates to the client"""

    first_net = request.form.get("first-net")
    second_net = request.form.get("second-net")

    if (
        "user_id" in session
        and session["user_id"] in pads_data
        and session["user_id"] in nets_data
        and session["user_id"] in dimensions_data
        and first_net in nets_data[session["user_id"]]
        and second_net in nets_data[session["user_id"]]
    ):
        user_id = session["user_id"]
        layout_dimensions = dimensions_data[user_id]
        first_coordinates = []
        second_coordinates = []

        x_offset = (layout_dimensions["height"] if BOARD_ORIENTATION == 90 else 0) + (layout_dimensions["width"] if BOARD_ORIENTATION == 180 else 0)
        y_offset = (layout_dimensions["height"] if BOARD_ORIENTATION == 180 else 0) + (layout_dimensions["height"] if BOARD_ORIENTATION == 270 else 0)

        for pads in pads_data[user_id]:
            for pad in pads:
                if not pad["is_allowed"]:
                    continue

                footprint_x = pad["footprint_x"]
                footprint_y = pad["footprint_y"]
                footprint_angle = pad["footprint_angle"] * math.pi / 180

                pad_x = pad["pad_x"]
                pad_y = pad["pad_y"]

                relative_angle = pad["relative_angle"]

                board_angle = BOARD_ORIENTATION * math.pi / 180

                print(footprint_x, footprint_y, footprint_angle, pad_x, pad_y, relative_angle, board_angle)

                local_pad_x = footprint_x + pad_x * math.cos(-1*footprint_angle) - pad_y * math.sin(-1*footprint_angle)
                local_pad_y = footprint_y + pad_x * math.sin(-1*footprint_angle) + pad_y * math.cos(-1*footprint_angle)

                rotated_pad_x = local_pad_x * math.cos(board_angle) - local_pad_y * math.sin(board_angle)
                rotated_pad_y = local_pad_x * math.sin(board_angle) + local_pad_y * math.cos(board_angle)

                candidate_pad_x = rotated_pad_x + x_offset
                candidate_pad_y = rotated_pad_y + y_offset


                if pad["net"] == first_net or pad["net"] == "/" + first_net:
                    first_coordinates.append([pad["id"], candidate_pad_x, candidate_pad_y, relative_angle, pad["net"]])

                if pad["net"] == second_net or pad["net"] == "/" + second_net:
                    second_coordinates.append([pad["id"], candidate_pad_x, candidate_pad_y, relative_angle, pad["net"]])

        if len(first_coordinates) > 0 and len(second_coordinates) > 0:
            x1 = [x[1] for x in first_coordinates]
            x2 = [x[1] for x in second_coordinates]
            delta1 = max(x1) - min(x2)
            delta2 = max(x2) - min(x1)
            print(x1)
            print(x2)
            if delta1 < delta2:
                left_index = x1.index(min(x1))
                right_index = x2.index(max(x2))
                left_coordinates = first_coordinates[left_index]
                right_coordinates = second_coordinates[right_index]
            else:
                left_index = x2.index(min(x2))
                right_index = x1.index(max(x1))
                left_coordinates = second_coordinates[left_index]
                right_coordinates = first_coordinates[right_index]
            candidates_data[user_id] = {
                "left": left_coordinates,
                "right": right_coordinates,
            }

            sio.emit("probe_candidates", candidates_data[user_id])

        else:
            sio.emit("net_selection_error", f"No valid candidates found for the selected nets (first: {len(first_coordinates)}, second: {len(second_coordinates)})")

    return "Selected"


@sio.on("board_orientation")
def board_orientation(orientation):
    """Updates the board orientation"""
    global BOARD_ORIENTATION
    BOARD_ORIENTATION = orientation
    # print(f"Board orientation: {BOARD_ORIENTATION}")
