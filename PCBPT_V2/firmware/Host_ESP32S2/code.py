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

Probe1 = Probe(0.0, 0.0)
Probe2 = Probe(0.0, 0.0)

while True:
    if supervisor.runtime.serial_bytes_available:
        command_correct = True
        movements       = []
        cmd = input().strip()

        # Sometimes Windows sends an extra (or missing) newline - ignore them
        if cmd == "":
            continue

        cmd_split = cmd.split()

        PARAMETER_CNT = len(cmd_split)

        if (PARAMETER_CNT > 4 or PARAMETER_CNT <= 1):
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
            if cmd_split[i][0] == 'A' or cmd_split[i][0] == 'B' or cmd_split[i][0] == 'X' or cmd_split[i][0] == 'Y':
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
            # print(movements[i])
            movement_float = float(movements[i])
            cmd_split_dict[cmd_split[i][0]] = "{:.3f}".format(movement_float)

        if "A" in cmd_split_dict:
            print(cmd_split_dict["A"])
        if "B" in cmd_split_dict:
            print(cmd_split_dict["B"])
        if "X" in cmd_split_dict:
            print(cmd_split_dict["X"])
        if "Y" in cmd_split_dict:
            print(cmd_split_dict["Y"])



