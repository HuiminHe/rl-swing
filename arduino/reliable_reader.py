import serial
from time import sleep, time


def startSerial(tty_id, baud=500000, timeout=0.05):
    ser = serial.Serial('/dev/ttyACM{}'.format(tty_id), baud, timeout=timeout)
    ser.close()
    ser.open()
    print("Waiting for initialization")
    sleep(5)
    if ser.isOpen():
        print(ser.portstr, ": connection successful.")
        return ser
    else:
        return False


def readSerial(ser):
    waiting = ser.in_waiting
    buffer = ser.read(waiting).decode("utf-8")  # read all char in buffer
    s = buffer.splitlines()
    if len(s) >= 2:
        return s[-2]
    else:
        return None


if __name__ == '__main__':
    for i_ep in range(3):
        ser = startSerial(0, 2000000)
        t = 0
        while t < 100:
            d = readSerial(ser)
            if d is not None:
                print(time(), t, d)
                t += 1
