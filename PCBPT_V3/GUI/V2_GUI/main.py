from kiutils.board import Board
from kiutils.libraries import LibTable
from kiutils.schematic import Schematic
from kiutils.footprint import Footprint
from kiutils.symbol import SymbolLib
from kiutils.wks import WorkSheet
from kiutils.dru import DesignRules
from kiutils.items.fpitems import FpText
import kiutils

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

PROBE_LASER_BIAS_X_L = 16.7
PROBE_LASER_BIAS_Y_L = 8.5

# PROBE_LASER_BIAS_X_L = 0
# PROBE_LASER_BIAS_Y_L = 0

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
        self.cali_probe_btn.clicked.connect(self.select_cali_pad)
        self.start_btn.clicked.connect(self.start_probing)

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

        self.file_select_btn.clicked.connect(self.select_file)
        self.rotate_btn.clicked.connect(self.rotate_replot_board)

        self.set_L_btn.clicked.connect(self.set_bias_L)

        self.PCB_fileName = ""

        self.EDGE_X = []
        self.EDGE_Y = []

        self.pads_x = []
        self.pads_y = []
        self.pads_size = []

        self.components_x = []
        self.components_y = []

        self.PADS = []
        self.NET_LIST = []
        self.PARTS = []

        self.SELECTED_PADS = []

        self.cadidate_pads_X = []
        self.cadidate_pads_Y = []

        self.FID_X = []
        self.FID_Y = []

        self.BOARD_ANGLE = 0

        self.SELECTED_NET_1 = ""
        self.SELECTED_NET_2 = ""

        self.board = ''

        self.bias_x_L = 0.0
        self.bias_y_L = 0.0
        self.bias_x_R = 0.0
        self.bias_y_R = 0.0

        self.netlist.setColumnCount(1)
        self.netlist.setColumnWidth(0, 345)

    def select_file(self):
        self.PCB_fileName, _ = QtWidgets.QFileDialog.getOpenFileName(self)
        self.file_path.setText(self.PCB_fileName)

        self.board = Board().from_file(self.PCB_fileName)

        self.EDGE_X = []
        self.EDGE_Y = []

        self.pads_x = []
        self.pads_y = []
        self.pads_size = []

        self.components_x = []
        self.components_y = []

        self.PADS = []
        self.NET_LIST = []
        self.PARTS = []

        self.SELECTED_PADS = []

        self.cadidate_pads_X = []
        self.cadidate_pads_Y = []

        self.FID_X = []
        self.FID_Y = []

        self.cali_L = {'x': 0.0, 'y': 0.0}
        self.cali_R = {'x': 0.0, 'y': 0.0}

        self.proc_PCB()
        self.plot_PCB()

        self.netlist.setRowCount(len(self.NET_LIST))

        for i in range(len(self.NET_LIST)):
            self.netlist.setItem(0, i, QtWidgets.QTableWidgetItem(self.NET_LIST[i]))

        # print(self.NET_LIST)

    def get_net(self, board_in):
        # Use a breakpoint in the code line below to debug your script.
        for network in board_in.nets:
            net = dict()
            net_name = network.name
            if (net_name != ''):
                net['name'] = network.name
                self.NET_LIST.append(net['name'])

    def get_pads(self, board_in):
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
                    temp_x, temp_y = self.calc_rotation(pad.position.X, pad.position.Y, rotation)
                    padpos['x'] = footprint.position.X + temp_x
                    padpos['y'] = footprint.position.Y + temp_y
                else:
                    padpos['x'] = footprint.position.X + pad.position.X
                    padpos['y'] = footprint.position.Y + pad.position.Y

                apad['pos'] = padpos
                apad['size'] = pad.size.X * pad.size.Y
                apad['type'] = pad.type
                apad['component'] = refernce  # blongs to which component
                # print(apad)

                self.PADS.append(apad)

    def get_footprint(self, board_in):
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

            self.PARTS.append(part)

    def get_outline(self, board_in):
        for graphic in board_in.graphicItems:
            if (graphic.layer == 'Edge.Cuts'):  # edge_cuts
                if isinstance(graphic, kiutils.items.gritems.GrRect):
                    self.EDGE_X.append(graphic.start.X)
                    self.EDGE_Y.append(-1 * graphic.start.Y)
                    self.EDGE_X.append(graphic.end.X)
                    self.EDGE_Y.append(-1 * graphic.start.Y)
                    self.EDGE_X.append(graphic.end.X)
                    self.EDGE_Y.append(-1 * graphic.end.Y)
                    self.EDGE_X.append(graphic.start.X)
                    self.EDGE_Y.append(-1 * graphic.end.Y)

                if isinstance(graphic, kiutils.items.gritems.GrLine):
                    self.EDGE_X.append(graphic.start.X)
                    self.EDGE_Y.append(-1 * graphic.start.Y)
                    self.EDGE_X.append(graphic.end.X)
                    self.EDGE_Y.append(-1 * graphic.end.Y)

                if isinstance(graphic, kiutils.items.gritems.GrPoly):
                    for pos in graphic.coordinates:
                        self.EDGE_X.append(pos.X)
                        self.EDGE_Y.append(-1 * pos.Y)

    def calc_rotation(self, x, y, angle):
        rad = -1 * angle / 180 * np.pi

        x_r = x * np.cos(rad) - y * np.sin(rad)
        y_r = x * np.sin(rad) + y * np.cos(rad)

        return x_r, y_r

    def select_network(self, selected_net):
        for pad in self.PADS:
            if pad['net'] == selected_net:
                self.SELECTED_PADS.append(pad)

    def tst_func(self):
        for pad in self.PADS:
            if pad['net'] == self.SELECTED_NET_1:
                self.cadidate_pads_X.append(pad['pos']['x'])
                self.cadidate_pads_Y.append(pad['pos']['y'])

    def get_co(self, PADS_in):
        for pad in PADS_in:
            # x_axis.append(pad['pos']['x'])
            pad['pos']['y'] = -1 * pad['pos']['y']

    def get_size(self, PADS_in):
        sizes = []

        for pad in PADS_in:
            sizes.append(pad['size'] * 20)

        return sizes

    def rotate_board(self, angle, co_x, co_y):
        data_len = len(co_x)
        for i in np.arange(data_len):
            co_x[i], co_y[i] = self.calc_rotation(co_x[i], co_y[i], angle)

    def rotate_board_2(self, angle, pads):
        for pad in pads:
            pad['pos']['x'], pad['pos']['y'] = self.calc_rotation(pad['pos']['x'], pad['pos']['y'], angle)

    def bias_co(self, pads_in, bias_x, bias_y):
        for pad in pads_in:
            pad['pos']['x'] -= bias_x
            pad['pos']['y'] -= bias_y

    def proc_PCB(self):
        self.EDGE_X = []
        self.EDGE_Y = []

        self.pads_x = []
        self.pads_y = []
        self.pads_size = []

        self.components_x = []
        self.components_y = []

        self.PADS = []
        self.NET_LIST = []
        self.PARTS = []

        self.SELECTED_PADS = []

        self.cadidate_pads_X = []
        self.cadidate_pads_Y = []

        self.FID_X = []
        self.FID_Y = []

        self.cali_L = {'x': 0.0, 'y': 0.0}
        self.cali_R = {'x': 0.0, 'y': 0.0}

        self.get_net(self.board)  # get the list of the network in the design
        self.get_pads(self.board)  # get all pads that connected with some signal
        self.get_footprint(self.board)
        self.get_outline(self.board)

        self.get_co(self.PADS)
        pads_s = self.get_size(self.PADS)
        self.rotate_board_2(self.BOARD_ANGLE, self.PADS)

        self.get_co(self.PARTS)
        self.rotate_board_2(self.BOARD_ANGLE, self.PARTS)

        self.EDGE_X.append(self.EDGE_X[0])
        self.EDGE_Y.append(self.EDGE_Y[0])
        self.rotate_board(self.BOARD_ANGLE, self.EDGE_X, self.EDGE_Y)

        board_bias_x = min(self.EDGE_X)
        board_bias_y = max(self.EDGE_Y)

        self.bias_co(self.PADS, board_bias_x, board_bias_y)
        self.bias_co(self.PARTS, board_bias_x, board_bias_y)

        self.EDGE_X -= board_bias_x
        self.EDGE_Y -= board_bias_y

        for pad in self.PADS:
            self.pads_x.append(pad['pos']['x'])
            self.pads_y.append(pad['pos']['y'])
            self.pads_size.append(pad['size'] * 5)

        for part in self.PARTS:
            self.components_x.append(part['pos']['x'])
            self.components_y.append(part['pos']['y'])

    def plot_PCB(self):
        self.ax.clear()
        self.ax.set_aspect('equal')
        self.ax.scatter(self.pads_x, self.pads_y, self.pads_size, c='r', marker='s')
        # self.ax.scatter(self.components_x, self.components_y, s=60, c='b', marker='+')
        self.ax.plot(self.EDGE_X, self.EDGE_Y, c='Black')

        self.canvas.draw()

    def probe(self, pos):
        self.plot_PCB()

        self.ax.scatter(pos['x'], pos['y'], s=120, facecolors='none', edgecolors='b')
        self.canvas.draw()

        self.cmd = 'P' + 'A' + "{:.2f}".format(pos['x'] + self.bias_x_L - PROBE_LASER_BIAS_X_L) + 'B' \
                   + "{:.2f}".format(-1 * pos['y'] + self.bias_y_L - PROBE_LASER_BIAS_Y_L) + '\n'
        self.ser.write(self.cmd.encode('utf-8'))

    def select_pad(self, signal):
        probe_pos = {'x': 0.0, 'y': 0.0}
        for pad in self.PADS:
            if (pad['net'] == signal):
                probe_pos['x'] = pad['pos']['x']
                probe_pos['y'] = pad['pos']['y']
                break

        self.probe(probe_pos)

    def start_probing(self):
        selecetd_sig = []
        for selected in self.netlist.selectedIndexes():
            selecetd_sig.append(selected.data())

        for sig in selecetd_sig:
            self.select_pad(sig)

    # select pads for calibration
    def select_cali_pad(self):
        cali_x_L = self.PADS[0]['pos']['x']
        cali_y_L = self.PADS[0]['pos']['y']
        cali_x_R = self.PADS[0]['pos']['x']
        cali_y_R = self.PADS[0]['pos']['y']

        for pad in self.PADS:
            if (pad['component'][0:3] == 'FID'):
                if (pad['pos']['x'] < cali_x_L):
                    cali_x_L = pad['pos']['x']
                    cali_y_L = pad['pos']['y']
                if (pad['pos']['x'] > cali_x_R):
                    cali_x_R = pad['pos']['x']
                    cali_y_R = pad['pos']['y']

        self.cali_L['x'] = cali_x_L
        self.cali_L['y'] = cali_y_L
        self.cali_R['x'] = cali_x_R
        self.cali_R['y'] = cali_y_R

        self.ax.scatter(self.cali_L['x'], self.cali_L['y'], s=120, facecolors='none', edgecolors='r')
        self.canvas.draw()

        self.cmd = 'C' + 'A' + "{:.2f}".format(self.cali_L['x'] + self.bias_x_L) + 'B' + "{:.2f}".format(-1*self.cali_L['y'] + self.bias_y_L) + '\n'
        self.ser.write(self.cmd.encode('utf-8'))

        # print(self.cali_L['x'], self.cali_L['y'])

        current_time = read_current_time()
        self.log.append(current_time + " > Please control the left Laser diode to the highlighted pad")

    def rotate_replot_board(self):
        self.BOARD_ANGLE += 90
        if (self.BOARD_ANGLE >= 360):
            self.BOARD_ANGLE -= 360

        self.proc_PCB()
        self.plot_PCB()

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
            self.log.append(current_time + " Opened @ " + str(self.serial_speed[index]) + "bps")

    def send_home_cmd(self):
        self.ser.write(self.HOME_CMD.encode('utf-8'))

    def send_tst_cmd(self):
        self.cmd = self.tst_cmd.displayText().upper() + '\n'
        self.ser.write(self.cmd.encode('utf-8'))

    def move_up_L(self):
        step_size = str(-1 * self.step_size_select_dia.value())
        self.cmd = 'T' + 'A0' + 'B' + step_size + '\n'
        self.ser.write(self.cmd.encode('utf-8'))

        self.bias_y_L += -1 * self.step_size_select_dia.value()

    def move_left_L(self):
        step_size = str(-1 * self.step_size_select_dia.value())
        self.cmd = 'T' + 'A' + step_size + 'B0' + '\n'
        self.ser.write(self.cmd.encode('utf-8'))

        self.bias_x_L += -1 * self.step_size_select_dia.value()

    def move_right_L(self):
        step_size = str(self.step_size_select_dia.value())
        self.cmd = 'T' + 'A' + step_size + 'B0' + '\n'
        self.ser.write(self.cmd.encode('utf-8'))

        self.bias_x_L += self.step_size_select_dia.value()

    def move_down_L(self):
        step_size = str(self.step_size_select_dia.value())
        self.cmd = 'T' + 'A0' + 'B' + step_size + '\n'
        self.ser.write(self.cmd.encode('utf-8'))

        self.bias_y_L += self.step_size_select_dia.value()

    def set_bias_L(self):
        pass
        # print("{:.2f}".format(self.bias_x_L), "{:.2f}".format(self.bias_y_L))

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    # creating apyqt5 application
    app = QApplication(sys.argv)
    # creating a window object
    main = MainWindow()
    # main.proc_PCB()
    # main.plot_PCB()
    # showing the window
    main.show()
    # loop
    sys.exit(app.exec_())








