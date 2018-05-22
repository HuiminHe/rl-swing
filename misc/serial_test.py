import serial

ser = serial.Serial('/dev/ttyACM0',9600)
while True:
    try:
        read_serial=ser.readline()
        print int(filter(str.isdigit, read_serial))
    except ValueError:
        pass
