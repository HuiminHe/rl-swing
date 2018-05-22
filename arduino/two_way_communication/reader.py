'''
The serial buffer can be annoying. There is no gaurantee to the buffer length to be read.
'''
import sys
import serial
import struct
import time

ser = serial.Serial('/dev/ttyACM{}'.format(sys.argv[1]), 500000, timeout=0.05)
buffer = ''
t = t_last = 0
step = 1
try:
    while True:
        waiting = ser.in_waiting
        try:
            buffer += str(ser.read(waiting), "utf-8") #read all char in buffer
            s = buffer.splitlines()
            if len(s) > 2:
                try:
                    data = [int(x) for x in s[-2].split(" ")]
                    t = data[0]
                    if t != t_last:
                        print(data)
                        ser.write(struct.pack('>BB', 127, 127))
                        t_last = t
                        step += 1
                        if step % 100 == 0:
                            buffer = ''
                except ValueError:
                    pass
        except serial.SerialException:
            pass
except KeyboardInterrupt:
    ser.close()
