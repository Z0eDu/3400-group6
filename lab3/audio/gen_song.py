#! /usr/bin/env python3

import wave
import struct
import sys

def wav_to_floats(wave_file):
    w = wave.open(wave_file)
    astr = w.readframes(w.getnframes())
    # convert binary chunks to short 
    a = struct.unpack("%ih" % (w.getnframes()* w.getnchannels()), astr)
    a = [float(val) / pow(2, 15) for val in a]
    return a

if __name__ == '__main__':
    # read the wav file specified as first command line arg
    signal = wav_to_floats(sys.argv[1])
    # print "read "+str(len(signal))+" frames"
    # print  "in the range "+str(min(signal))+" to "+str(min(signal))
    for x in signal:
        print(hex(int(127*x + 128))[2:].zfill(2))
