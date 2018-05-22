from __future__ import division
from __future__ import print_function

import sys
sys.path.append('..')
from joystick import JoyStick
from servo import ServoController

if __name__ == '__main__':
    sc = ServoController(angle_range=80)
    js = JoyStick()
    while True:
        # read serial
        v = js.read()
        # convert to servo angle
        angle = sc.analog2deg(v)
        print(angle)
        for i in range(len(angle)):
            sc.control([i, angle[i]])
