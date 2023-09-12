from kiutils.board import Board
from kiutils.libraries import LibTable
from kiutils.schematic import Schematic
from kiutils.footprint import Footprint
from kiutils.symbol import SymbolLib
from kiutils.wks import WorkSheet
from kiutils.dru import DesignRules
from kiutils.items.fpitems import FpText

import matplotlib.pyplot as plt

from os import path

def get_net(board_in):
    # Use a breakpoint in the code line below to debug your script.
    for network in board_in.nets:
        net = dict()
        net_name = network.name
        if (net_name != ''):
            net['name'] = network.name
            NET_LIST.append(net)

    print(NET_LIST)
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

    print(PARTS)

def get_pads(board_in):
    for footprint in board_in.footprints:
        pads = footprint.pads
        for pad in pads:
            print(pad)
            apad = dict()
            if (pad.net != None):
                padpos = dict()
                apad['net'] = pad.net.name
                padpos['x'] = footprint.position.X + pad.position.X
                padpos['y'] = footprint.position.Y + pad.position.Y
                apad['pos'] = padpos
                apad['size'] = pad.size.X * pad.size.Y
                PADS.append(apad)

    print(PADS)

def tst_func(board_in):
    for footprint in board_in.footprints:
        pads = footprint.pads
        for pad in pads:
            print(pad)

def get_co(PADS_in):
    x_axis = []
    y_axis = []
    for pad in PADS_in:
        x_axis.append(pad['pos']['x'])
        y_axis.append(pad['pos']['y'])

    return x_axis, y_axis

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

    # tst_func(board)
    # get_footprint(board)

    # plot
    fig, ax = plt.subplots()
    x, y = get_co(PADS)

    ax.scatter(x, y, s=30, c='r')

    plt.show()


