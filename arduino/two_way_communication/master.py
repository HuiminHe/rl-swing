import serial
import struct
from time import sleep

ser = serial.Serial('/dev/ttyACM0', 2000000, timeout=1)
count = 1
try:
    while True:
        ser.write(struct.pack('>BBB', count, count+10, count+20))
        count += 1
        if count > 100:
            count = 0
        sleep(0.005)
except KeyboardInterrupt:
    ser.close()
    print('Connection interrupted')
