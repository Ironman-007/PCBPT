from kiutils.board import Board
from kiutils.libraries import LibTable
from kiutils.schematic import Schematic
from kiutils.footprint import Footprint
from kiutils.symbol import SymbolLib
from kiutils.wks import WorkSheet
from kiutils.dru import DesignRules
from kiutils.items.fpitems import FpText

from os import path

def get_net(board_in):
    # Use a breakpoint in the code line below to debug your script.
    for network in board_in.nets:
        print(network)

def get_components(sch_in):
    # Use a breakpoint in the code line below to debug your script.
    for symbol in sch_in.schematicSymbols:
        print(symbol)

def get_footprint(board_in):
    # Use a breakpoint in the code line below to debug your script.
    for footprint in board_in.footprints:
        pad = footprint.pads
        print(len(pad))
        print(pad[0].position)
        # if (fptext[0].type == 'reference'):
        #     print(fptext[0].text)

board_path = "C:\DATA\Documents\Lab\github\PCBPT\PCBPT_V3\PCB\Board_To_Be_Tested_V2\Board_To_Be_Tested.kicad_pcb"
board_sch_path = "C:\DATA\Documents\Lab\github\PCBPT\PCBPT_V3\PCB\Board_To_Be_Tested_V2\Board_To_Be_Tested.kicad_sch"

board = Board().from_file(board_path)
sch   = Schematic().from_file(board_sch_path)

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    # get_net(board)
    # get_components(sch)
    get_footprint(board)

