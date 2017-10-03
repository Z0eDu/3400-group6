#! /usr/bin/env python3

from math import sin
from math import pi


if __name__ == '__main__':
    for i in range(256):
        print(hex(int(127*sin(2*pi*i / 256) + 128))[2:].zfill(2))
