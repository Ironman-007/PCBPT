from kiutils.board import Board
from kiutils.libraries import LibTable
from kiutils.schematic import Schematic
from kiutils.footprint import Footprint
from kiutils.symbol import SymbolLib
from kiutils.wks import WorkSheet
from kiutils.dru import DesignRules
from kiutils.items.fpitems import FpText

import matplotlib.pyplot as plt
from matplotlib.patches import Circle
from matplotlib.patheffects import withStroke
import numpy as np

import sys
from PyQt5 import QtCore, QtWidgets, uic, QtGui
from pyqtgraph import PlotWidget
from PyQt5.QtWidgets import QApplication, QVBoxLayout, QWidget, QFrame
import pyqtgraph as pg

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
import os
import serial
from os import path
import datetime

board_path = "C:\DATA\Documents\Lab\github\PCBPT\PCBPT_V3\PCB\Board_To_Be_Tested_V2\Board_To_Be_Tested.kicad_pcb"
board_sch_path = "C:\DATA\Documents\Lab\github\PCBPT\PCBPT_V3\PCB\Board_To_Be_Tested_V2\Board_To_Be_Tested.kicad_sch"

board = Board().from_file(board_path)
sch   = Schematic().from_file(board_sch_path)

PADS     = []
NET_LIST = []
PARTS    = []
EDGE_X   = []
EDGE_Y   = []

SELECTED_PADS = []

cadidate_pads_X = []
cadidate_pads_Y = []

BOARD_ANGLE = 0

SELECTED_NET_1 = '+3V3'
SELECTED_NET_2 = '/DIN'

FID_X = []
FID_Y = []

def get_net(board_in):
    # Use a breakpoint in the code line below to debug your script.
    for network in board_in.nets:
        net = dict()
        net_name = network.name
        if (net_name != ''):
            net['name'] = network.name
            NET_LIST.append(net['name'])

    # print(NET_LIST)

    # for net_name in NET_LIST:
    #     print(net_name['name'])
        # print(network)

# def get_components(sch_in):
#     # Use a breakpoint in the code line below to debug your script.
#     for symbol in sch_in.schematicSymbols:
#         print(symbol)

def get_footprint(board_in):
    for footprint in board_in.footprints:
        part = dict()
        part_pos = dict()
        part_pos['x'] = footprint.position.X
        part_pos['y'] = footprint.position.Y
        part['pos'] = part_pos
        for text in (footprint.graphicItems):
            # print(type(text))
            pass
            # if (text['type'] == 'reference'):
            #     part['ref'] = text['text']

        PARTS.append(part)

    # print(PARTS)

def calc_rotation(x, y, angle):
    rad = -1*angle/180 * np.pi

    x_r = x * np.cos(rad) - y * np.sin(rad)
    y_r = x * np.sin(rad) + y * np.cos(rad)

    return x_r, y_r

def get_pads(board_in):
    for footprint in board_in.footprints:
        for graph in footprint.graphicItems:
            if (type(graph) is FpText):
                if (graph.type == 'reference'):
                    refernce = graph.text
                    break
        pads = footprint.pads
        for pad in pads:
            # print(pad)
            apad = dict()
            # if (pad.net != None):
            padpos = dict()
            if (pad.net == None):
                apad['net'] = 'None'
            else:
                apad['net'] = pad.net.name
            rotation = footprint.position.angle
            if (rotation != None):
                temp_x, temp_y = calc_rotation(pad.position.X, pad.position.Y, rotation)
                padpos['x'] = footprint.position.X + temp_x
                padpos['y'] = footprint.position.Y + temp_y
            else:
                padpos['x'] = footprint.position.X + pad.position.X
                padpos['y'] = footprint.position.Y + pad.position.Y

            apad['pos']  = padpos
            apad['size'] = pad.size.X * pad.size.Y
            apad['type'] = pad.type
            apad['component']  = refernce # blongs to which component
            # print(apad)

            PADS.append(apad)

def select_network(selected_net):
    for pad in PADS:
        if pad['net'] == selected_net:
            SELECTED_PADS.append(pad)

    # print(SELECTED_PADS)

def tst_func():
    for pad in PADS:
        if pad['net'] == SELECTED_NET_1:
            print(pad)
            cadidate_pads_X.append(pad['pos']['x'])
            cadidate_pads_Y.append(pad['pos']['y'])
    # for footprint in board_in.footprints:
        # print(footprint)
        # pass

def get_co(PADS_in):
    # x_axis = []
    # y_axis = []

    for pad in PADS_in:
        # x_axis.append(pad['pos']['x'])
        pad['pos']['y'] = -1*pad['pos']['y']

    # return x_axis, y_axis

def get_size(PADS_in):
    sizes = []

    for pad in PADS_in:
        sizes.append(pad['size']*20)

    return sizes

def get_outline(board_in):
    for graphic in board_in.graphicItems:
        # print(graphic)
        if(graphic.layer == 'Edge.Cuts'): # edge_cuts
            for pos in graphic.coordinates:
                EDGE_X.append(pos.X)
                EDGE_Y.append(-1*pos.Y)

    # print(EDGE_X, EDGE_Y)
            # print(graphic)

def rotate_board(angle, co_x, co_y):
    data_len = len(co_x)
    for i in np.arange(data_len):
        co_x[i], co_y[i] = calc_rotation(co_x[i], co_y[i], angle)

def rotate_board_2(angle, pads):
    for pad in pads:
        pad['pos']['x'], pad['pos']['y'] = calc_rotation(pad['pos']['x'], pad['pos']['y'], angle)

def bias_co(pads_in, bias_x, bias_y):
    for pad in pads_in:
        pad['pos']['x'] -= bias_x
        pad['pos']['y'] -= bias_y

    # for i in np.arange(data_len):
    #     co_x[i], co_y[i] = calc_rotation(co_x[i], co_y[i], angle)

def read_current_time():
    now = datetime.datetime.now(datetime.timezone.utc)
    current_time = now.strftime("%Z:%j/%H:%M:%S")
    return current_time

class MainWindow(QtWidgets.QDialog):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.setFixedSize(760, 827)
        # Load the UI Page
        uic.loadUi('v2_gui.ui', self)
        self.figure, self.ax = plt.subplots()
        self.canvas = FigureCanvas(self.figure)
        self.plot_layout.addWidget(self.canvas)

        self.scan_serial_btn.clicked.connect(self.scan)
        self.open_serial_btn.clicked.connect(self.open_port)
        self.send_cmd_btn.clicked.connect(self.send_tst_cmd)

        self.serial_ports_list = []
        self.serial_speed = [115200]

        # self.timer = QtCore.QTimer(self, interval=10, timeout=self.read_port)
        self.ser = serial.Serial()

        self.home_btn.clicked.connect(self.send_home_cmd)

        self.up_L.clicked.connect(self.move_up_L)
        self.left_L.clicked.connect(self.move_left_L)
        self.right_L.clicked.connect(self.move_right_L)
        self.down_L.clicked.connect(self.move_down_L)

        self.HOME_CMD = "H\n"
        self.cmd      = ""

    def plot_PCB(self, pads_x, pads_y):
        self.ax.clear()
        self.ax.set_aspect('equal')
        self.ax.scatter(pads_x, pads_y, pads_size, c='r', marker='s')
        self.ax.scatter(components_x, components_y, s=60, c='b', marker='+')
        self.ax.plot(edge_x, edge_y, c='Black')

        self.canvas.draw()

    def scan(self):
        if os.name == 'nt':  # sys.platform == 'win32':
            from serial.tools.list_ports_windows import comports
        elif os.name == 'posix':
            from serial.tools.list_ports_posix import comports

        for info in comports(False):
            port, desc, hwid = info
        iterator = sorted(comports(False))

        self.serial_ports_list = []  # clear the list first
        for n, (port, desc, hwid) in enumerate(iterator, 1):
            self.serial_ports_list.append("{:20} ".format(port))

        ports_num = len(self.serial_ports_list)

        self.serial_comboBox.clear()  # clear the list first
        for x in self.serial_ports_list:
            self.serial_comboBox.addItem(x)

    def open_port(self):
        index = self.serial_comboBox.currentIndex()
        serial_ports_port = self.serial_ports_list[index][:-1]  # delete the \n at the end
        # index = self.speed_comboBox.currentIndex()
        self.ser = serial.Serial(serial_ports_port, 115200, timeout=10)
        if (self.ser.is_open == True):
            current_time = read_current_time()
            self.log.append(current_time + self.ser.name + " Opened @ " + str(self.serial_speed[index]) + "bps")

    def send_home_cmd(self):
        self.ser.write(self.HOME_CMD.encode('utf-8'))

    def send_tst_cmd(self):
        self.cmd = self.tst_cmd.displayText().upper() + '\n'
        self.ser.write(self.cmd.encode('utf-8'))

    def move_up_L(self):
        step_size = str(-1 * self.step_size_select_dia.value())
        self.cmd = 'T' + 'A0' + 'B' + step_size + '\n'
        print(self.cmd)
        self.ser.write(self.cmd.encode('utf-8'))

    def move_left_L(self):
        step_size = str(-1 * self.step_size_select_dia.value())
        self.cmd = 'T' + 'A' + step_size + 'B0' + '\n'
        print(self.cmd)
        self.ser.write(self.cmd.encode('utf-8'))

    def move_right_L(self):
        step_size = str(self.step_size_select_dia.value())
        self.cmd = 'T' + 'A' + step_size + 'B0' + '\n'
        print(self.cmd)
        self.ser.write(self.cmd.encode('utf-8'))

    def move_down_L(self):
        step_size = str(self.step_size_select_dia.value())
        self.cmd = 'T' + 'A0' + 'B' + step_size + '\n'
        print(self.cmd)
        self.ser.write(self.cmd.encode('utf-8'))

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    get_net(board)  # get the list of the network in the design
    get_pads(board) # get all pads that connected with some signal
    get_footprint(board)
    get_outline(board)

    # plot
    fig, ax = plt.subplots()

    # pads_x, pads_y = get_co(PADS)
    get_co(PADS)
    pads_s    = get_size(PADS)
    rotate_board_2(BOARD_ANGLE, PADS)

    get_co(PARTS)
    rotate_board_2(BOARD_ANGLE, PARTS)

    EDGE_X.append(EDGE_X[0])
    EDGE_Y.append(EDGE_Y[0])
    edge_x, edge_y = EDGE_X, EDGE_Y
    rotate_board(BOARD_ANGLE, edge_x, edge_y)

    board_bias_x = min(edge_x)
    board_bias_y = max(edge_y)

    bias_co(PADS, board_bias_x, board_bias_y)
    bias_co(PARTS, board_bias_x, board_bias_y)

    edge_x -= board_bias_x
    edge_y -= board_bias_y

    pads_x = []
    pads_y = []
    pads_size = []

    components_x = []
    components_y = []

    for pad in PADS:
        pads_x.append(pad['pos']['x'])
        pads_y.append(pad['pos']['y'])
        pads_size.append(pad['size']*5)

    for part in PARTS:
        components_x.append(part['pos']['x'])
        components_y.append(part['pos']['y'])

    ax.scatter(pads_x, pads_y, s=pads_size, c='r', marker='s')
    ax.scatter(components_x, components_y, s=60, c='b', marker='+')
    ax.plot(edge_x, edge_y, c='Black')

    SELECTED_PADS = []
    select_network('+3V3')
    selected_pads_size = []

    for selected_pad in SELECTED_PADS:
        cadidate_pads_X.append(selected_pad['pos']['x'])
        cadidate_pads_Y.append(selected_pad['pos']['y'])
        selected_pads_size.append(selected_pad['size']*20)

    ax.scatter(cadidate_pads_X, cadidate_pads_Y, s = selected_pads_size,c='k', marker='P')

    FID = []
    for pad in PADS:
        if pad['component'][0:3] == 'FID':
            c = Circle((pad['pos']['x'], pad['pos']['y']),
                       radius=1, clip_on=False, zorder=10, linewidth=2.5,
                       edgecolor='b', facecolor='none',
                       path_effects=[withStroke(linewidth=7, foreground='white')])
            ax.add_artist(c)
            print(pad)

    plt.show()

    # creating apyqt5 application
    app = QApplication(sys.argv)
    # creating a window object
    main = MainWindow()
    main.plot_PCB(pads_x, pads_y)
    # showing the window
    main.show()
    # loop
    sys.exit(app.exec_())


