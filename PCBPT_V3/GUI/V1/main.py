from kiutils.board import Board
from kiutils.libraries import LibTable
from kiutils.schematic import Schematic
from kiutils.footprint import Footprint
from kiutils.symbol import SymbolLib
from kiutils.wks import WorkSheet
from kiutils.dru import DesignRules
from kiutils.items.fpitems import FpText

import matplotlib.pyplot as plt
import numpy as np

from os import path

def get_net(board_in):
    # Use a breakpoint in the code line below to debug your script.
    for network in board_in.nets:
        net = dict()
        net_name = network.name
        if (net_name != ''):
            net['name'] = network.name
            NET_LIST.append(net)

    # print(NET_LIST)
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
            print(apad)

            PADS.append(apad)

    print(PADS)

def tst_func(board_in):
    for footprint in board_in.footprints:
        # print(footprint)
        pass

def get_co(PADS_in):
    x_axis = []
    y_axis = []

    for pad in PADS_in:
        x_axis.append(pad['pos']['x'])
        y_axis.append(-1*pad['pos']['y'])

    return x_axis, y_axis

def get_size(PADS_in):
    sizes  = []

    for pad in PADS_in:
        sizes.append(pad['size']*20)

    return sizes

board_path = "C:\DATA\Documents\Lab\github\PCBPT\PCBPT_V3\PCB\Board_To_Be_Tested_V2\Board_To_Be_Tested.kicad_pcb"
board_sch_path = "C:\DATA\Documents\Lab\github\PCBPT\PCBPT_V3\PCB\Board_To_Be_Tested_V2\Board_To_Be_Tested.kicad_sch"

board = Board().from_file(board_path)
sch   = Schematic().from_file(board_sch_path)

PADS     = []
NET_LIST = []
PARTS    = []

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    get_net(board)  # get the list of the network in the design
    get_pads(board) # get all pads that connected with some signal
    get_footprint(board)

    # plot
    fig, ax = plt.subplots()

    x, y = get_co(PADS)
    s    = get_size(PADS)
    ax.scatter(x, y, s=s, c='r', marker='s')

    x, y = get_co(PARTS)
    ax.scatter(x, y, s=60, c='b', marker='+')

    # ax.scatter([0], [0], s=100, c='k')

    plt.show()


