import board
import busio
import time
import supervisor
import digitalio

# i2c = busio.I2C(board.SCL, board.SDA)

led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT

print("PCBPT connected!")

PARAMETER_CNT = 2

cmd_split_dict = {}
movements      = []

addr_A = 0x40
addr_B = 0x41
addr_X = 0x42
addr_Y = 0x43
addr_M = 0x44
addr_N = 0x45

steps2move_A   = 0
steps2move_B   = 0
steps2move_X   = 0
steps2move_Y   = 0
steps2move_M   = 0
steps2move_N   = 0

comm_A_exist   = False
comm_B_exist   = False
comm_X_exist   = False
comm_Y_exist   = False

command_correct = True

class Probe:
  def __init__(self, Init_Coor_X, Init_Coor_Y):
    self.Current_X = Init_Coor_X
    self.Current_Y = Init_Coor_Y

    self.Target_X  = Init_Coor_X
    self.Target_Y  = Init_Coor_Y

    self.coe       = 0.0

def flash_led(t):
    led.value = True
    time.sleep(t)
    led.value = False

def check_if_number(str_input):
    for i in range(len(str_input)):
        if (str_input[i] >= '0' and str_input[i] <= '9' or str_input[i] == '.'):
            continue
        else:
            return False

    return True

def calculate_steps(distance):
    print(distance)

    return True

def send_cmd(address, data):
    print(address)
    print(data[0])
    print(data[1])
    print(data[2])
    print(data[3])

    return True

Probe1 = Probe(0.0, 0.0)
Probe2 = Probe(0.0, 0.0)

while True:
    if supervisor.runtime.serial_bytes_available:
        command_correct = True
        comm_A_exist    = False
        comm_B_exist    = False
        comm_X_exist    = False
        comm_Y_exist    = False

        movements       = []
        cmd = input().strip()

        # Sometimes Windows sends an extra (or missing) newline - ignore them
        if cmd == "":
            continue

        if cmd[0] == "P" or cmd[0] == "C":
            pass
        else:
            command_correct == False
            print("!!! Wrong command - command should begin with 'P' or 'C'.")
            continue

        cmd_split = cmd.split()

        PARAMETER_CNT = len(cmd_split)

        if (PARAMETER_CNT > 5 or PARAMETER_CNT <= 2):
            print("!!! Wrong command - got %2d parameters, should input 4 parameters." %(PARAMETER_CNT))
            continue

        flash_led(0.05)
        cmd_split_dict = {}

        for i in range(PARAMETER_CNT):
            movement_temp = cmd_split[i][1:]
            if check_if_number(movement_temp):
                movements.append(movement_temp)
            else:
                command_correct = False
                print("!!! Wrong command - only accept number")
                continue

        for i in range(PARAMETER_CNT):
            if cmd_split[i][0] == 'P' or cmd_split[i][0] == 'C' or cmd_split[i][0] == 'A' or cmd_split[i][0] == 'B' or cmd_split[i][0] == 'X' or cmd_split[i][0] == 'Y':
                continue
            else:
                command_correct = False
                print("!!! Wrong command - only accept 'A' or 'B' or 'X' or 'Y'")
                print("!!! You input includes %c" %(cmd_split[i][0]))
                continue

        if (command_correct == False):
            continue

        for i in range(PARAMETER_CNT):
            cmd_split_dict[cmd_split[i][0]] = 0

        for i in range(PARAMETER_CNT):
            if (len(movements[i]) > 0):
                # print(movements[i])
                movement_float = float(movements[i])
                cmd_split_dict[cmd_split[i][0]] = "{:.3f}".format(movement_float)

        if "A" in cmd_split_dict:
            comm_A_exist =  True
            A_move = cmd_split_dict["A"]
        if "B" in cmd_split_dict:
            comm_B_exist =  True
            B_move = cmd_split_dict["B"]

        if "X" in cmd_split_dict:
            comm_X_exist =  True
            X_move = cmd_split_dict["X"]
        if "Y" in cmd_split_dict:
            comm_Y_exist =  True
            Y_move = cmd_split_dict["Y"]

        if (comm_A_exist == True and comm_B_exist == True):
            print("Probe #1 move to ", (A_move, B_move))
            Probe1.Target_X = A_move
            # if (Probe1.Target_X == Probe1.Current_X):
            #   pass
            # else:
            #   steps = calculate_steps(Probe1.Target_X - Probe1.Current_X)
            #   send_cmd(addr_A, steps)
            #   Probe1.Current_X = A_move
            Probe1.Target_Y = B_move
            # if (Probe1.Target_Y == Probe1.Current_Y):
            #   pass
            # else:
            #   steps = calculate_steps(Probe1.Target_Y - Probe1.Current_Y)
            #   send_cmd(addr_B, steps)
            #   Probe1.Current_Y = B_move
        if (comm_X_exist == True and comm_Y_exist == True):
            print("Probe #2 move to ", (X_move, Y_move))
            Probe2.Target_Y = X_move
            # if (Probe2.Target_X == Probe2.Current_X):
            #   pass
            # else:
            #   steps = calculate_steps(Probe2.Target_X - Probe2.Current_X)
            #   send_cmd(addr_X, steps)
            #   Probe2.Current_X = X_move
            Probe2.Target_Y = Y_move
            # if (Probe2.Target_X == Probe2.Current_X):
            #   pass
            # else:
            #   steps = calculate_steps(Probe2.Target_Y - Probe2.Current_Y)
            #   send_cmd(addr_Y, steps)
            #   Probe2.Current_Y = Y_move




