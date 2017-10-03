#! /usr/bin/env python3

from math import sin
from math import pi
import sys

if __name__ == '__main__':
    with open(sys.argv[1]) as f:
        for line in f.readlines():
            print(hex(int(2**32*float(line)/25e6))[2:].zfill(8))
