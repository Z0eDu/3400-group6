#! /usr/bin/env python3

import os, sys
from PIL import Image
import argparse
import tempfile

size = (120, 120)

def main():
    parser = argparse.ArgumentParser(description='convert image to 120x120 verilog ROM file')
    parser.add_argument('img', help='image file')
    args = parser.parse_args()

    im = Image.open(args.img)
    im.thumbnail(size, Image.ANTIALIAS)
    for y in range(0, 128):
        for x in range(0, 128):
            color = (0, 0, 0)
            if x < im.size[0] and y < im.size[1]:
                color = im.getpixel((x,y))[0:3]
            r, g, b = color
            # print(color)
            hexc = (r >> 5) << 5 | (g >> 5) << 3 | (b >> 6)
            print(hex(hexc)[2:])

if __name__ == '__main__':
    main()
