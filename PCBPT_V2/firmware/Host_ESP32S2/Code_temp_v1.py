import board
import busio
import time
import supervisor
import digitalio

# i2c = busio.I2C(board.SCL, board.SDA)

led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT

print("PCBPT connected!")

PARAMETER_CNT = 8

cmd_split_dict = {}
movements      = []

command_correct = True

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

while True:
    if supervisor.runtime.serial_bytes_available:
        command_correct = True
        movements       = []
        cmd = input().strip()

        # Sometimes Windows sends an extra (or missing) newline - ignore them
        if cmd == "":
            continue

        cmd_split = cmd.split()

        if (len(cmd_split) != 8):
            print("!!! Wrong command - got %2d parameters, should input 8 parameters." %(len(cmd_split)))
            continue

        flash_led(0.05)
        cmd_split_dict = {}

        for i in range(PARAMETER_CNT):
            movement_temp = cmd_split[i][1:]
            if check_if_number(movement_temp):
                movements.append(movement_temp)
            else:
                command_correct = False
                print("!!!  Wrong command - only accept number")
                continue

        if (command_correct == False):
            continue

        for i in range(PARAMETER_CNT):
            cmd_split_dict[cmd_split[i][0]] = 0

        print("keep going")
        print(movements)

        for i in range(PARAMETER_CNT):
            print(movements[i])
            movement_float = float(movements[i])
            cmd_split_dict[cmd_split[i][0]] = "{:.3f}".format(movement_float)

        print(cmd_split_dict)




