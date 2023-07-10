from ws2812 import WS2812
import utime
import machine
import sys
import select

power = machine.Pin(11,machine.Pin.OUT)
power.value(1)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
YELLOW = (255, 150, 0)
GREEN = (0, 255, 0)
CYAN = (0, 255, 255)
BLUE = (0, 0, 255)
PURPLE = (180, 0, 255)
WHITE = (255, 255, 255)
COLORS = (BLACK, RED, YELLOW, GREEN, CYAN, BLUE, PURPLE, WHITE)

led = WS2812(12,1) #WS2812(pin_num,led_count)

while True:
    #while sys.stdin in select.select([sys.stdin], [], [], 0)[0]:        
        ch = sys.stdin.readline()
        print(ch)
        if (ch[0] == 'r'):
            print("Beautiful color")
            for color in COLORS: 
                led.pixels_fill(color)
                led.pixels_show()
                utime.sleep(0.2)

