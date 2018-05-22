#####################################
# test incremental control of servo #
#####################################
from __future__ import division
from __future__ import print_function
import sys
sys.path.append('./..')
import servo

if __name__ == '__main__':
    sc = ServoController(50, 30, -5)
    
    for i in range(5):
        sc.increaseAngle(0)
