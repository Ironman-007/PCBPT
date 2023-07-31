import board
import busio
import time
import supervisor
import digitalio
import struct

Coral_addr_1 = '0x32'
Coral_addr_2 = '0x33'
Coral_addr_3 = '0x34'
Coral_addr_4 = '0x35'
Coral_addr_5 = '0x36'
Coral_addr_6 = '0x37'

IND1 = digitalio.DigitalInOut(board.IO21)
IND2 = digitalio.DigitalInOut(board.IO35)
IND3 = digitalio.DigitalInOut(board.IO33)
IND4 = digitalio.DigitalInOut(board.IO18)
IND5 = digitalio.DigitalInOut(board.IO17)
IND6 = digitalio.DigitalInOut(board.IO16)
led  = digitalio.DigitalInOut(board.LED)

# i2c = busio.I2C(board.SCL, board.SDA)

PARAMETER_CNT = 2

cmd_split_dict = {}
movements      = []

START_BYTE = 'S'

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

def Pins_init():
    IND1.direction = digitalio.Direction.OUTPUT
    IND2.direction = digitalio.Direction.OUTPUT
    IND3.direction = digitalio.Direction.OUTPUT
    IND4.direction = digitalio.Direction.OUTPUT
    IND5.direction = digitalio.Direction.OUTPUT
    IND6.direction = digitalio.Direction.OUTPUT
    led.direction  = digitalio.Direction.OUTPUT
    
    IND1.value = False
    IND2.value = False
    IND3.value = False
    IND4.value = False
    IND5.value = False
    IND6.value = False
    led.value  = False
    
    return True

def scan_Coral_modules():
    global i2c
    i2c = busio.I2C(board.IO39, board.IO37, frequency = 100_000)

    while not i2c.try_lock():
        pass

    Coral_I2C_addr = [hex(x) for x in i2c.scan()]

    if Coral_addr_1 in Coral_I2C_addr:
        IND1.value = True
        print('Found Coral #1')
    if Coral_addr_2 in Coral_I2C_addr:
        IND2.value = True
        print('Found Coral #2')
    if Coral_addr_3 in Coral_I2C_addr:
        IND3.value = True
        print('Found Coral #3')
    if Coral_addr_4 in Coral_I2C_addr:
        IND4.value = True
        print('Found Coral #4')
    if Coral_addr_5 in Coral_I2C_addr:
        IND5.value = True
        print('Found Coral #5')
    if Coral_addr_6 in Coral_I2C_addr:
        IND6.value = True
        print('Found Coral #6')

def check_if_number(str_input):
    for i in range(len(str_input)):
        if (str_input[i] >= '0' and str_input[i] <= '9' or str_input[i] == '.' or str_input[i] == '-'):
            continue
        else:
            return False

    return True

def cobs_encoding(buffer, size, encodedBuffer):
    read_index  = 0
    write_index = 1
    code_index  = 0
    code        = 1

    while (read_index < size):
        if (buffer[read_index] == 0):
            encodedBuffer[code_index] = code
            code = 1
            code_index = write_index
            write_index += 1
            read_index += 1

        else:
            encodedBuffer[write_index] = buffer[read_index]
            write_index += 1
            read_index += 1
            code += 1

            if (code == 0xFF):
                encodedBuffer[code_index] = code
                code = 1
                code_index = write_index
                write_index += 1

    encodedBuffer[code_index] = code

    return write_index

def send_cmd(addr, pos_bytes):
    print("addr: ", addr)
    print("pos_bytes: ", pos_bytes)

    Bytes = bytes(pos_bytes)

    COBS_bytes = [0] * 5
    cobs_encoding(Bytes, 4, COBS_bytes)
    print(COBS_bytes)

    addr_int = int(addr)

    i2c.writeto(addr_int, bytes(COBS_bytes))
    i2c.writeto(addr_int, bytes([0]))

    return True

Probe1 = Probe(0.0, 0.0)
Probe2 = Probe(0.0, 0.0)

# initialize all IOs to be used
Pins_init()

if supervisor.runtime.serial_connected:
    flash_led(0.05)
    print("PCBPT connected!")

# Sca I2C bus and find all Coral modules
scan_Coral_modules()

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
            A_pos = cmd_split_dict["A"]
        if "B" in cmd_split_dict:
            comm_B_exist =  True
            B_pos = cmd_split_dict["B"]

        if "X" in cmd_split_dict:
            comm_X_exist =  True
            X_pos = cmd_split_dict["X"]
        if "Y" in cmd_split_dict:
            comm_Y_exist =  True
            Y_pos = cmd_split_dict["Y"]

        if (comm_A_exist == True and comm_B_exist == True):
            print("Probe #1 move to ", (A_pos, B_pos))
            Probe1.Target_X = float(A_pos)
            print(Probe1.Target_X)

            if (Probe1.Target_X == Probe1.Current_X):
                print("Probe #1 X not changing")
                pass
            else:
              # Translate Probe1.Target_X to bytes array pos_bytes
              pos_bytes = bytearray(struct.pack("f", Probe1.Target_X))  
              # send it to Probe1
              send_cmd(Coral_addr_1, pos_bytes)
              Probe1.Current_X = Probe1.Target_X

            # Probe1.Target_Y = B_pos
            # if (Probe1.Target_Y == Probe1.Current_Y):
            #     print("Probe #1 Y not changing")
            #     pass
            # else:
            #   steps = calculate_steps(Probe1.Target_Y - Probe1.Current_Y)
            #   send_cmd(addr_B, steps)
            #   Probe1.Current_Y = B_move
        if (comm_X_exist == True and comm_Y_exist == True):
            print("Probe #2 move to ", (X_pos, Y_pos))
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

