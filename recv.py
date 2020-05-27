#!/usr/bin/python3
import struct
import array
import serial
import time

BUF = bytearray(21)
count = 0

def unpackData(buf):
    if buf[0] == 0x52 and buf[1] == 0x71:
        global count
        checksum = 0
        for i in buf[:-1]:
            checksum += i
            checksum = (checksum & 0xff)
        if checksum == buf[-1]:
            count = count + 1
            print("get %dth package" % count)
            data = struct.unpack('<4f',buf[4:20])
            print("dist = %f \n angle = %f \n maxLineVel = %f \n maxAngleVel = %f" % data)


def main():
    global count
    ser1 = serial.Serial('/dev/ttyTHS1',
                          baudrate =115200,
                          bytesize=serial.EIGHTBITS,
                          parity=serial.PARITY_NONE,
                          stopbits=serial.STOPBITS_ONE,
                          timeout = 1)
    while True:
        BUF = ser1.read(21)
        #print(len(BUF))
        print(BUF)
        if len(BUF) == 21:
            #print(BUF)
            unpackData(BUF)


if __name__ == "__main__":
    main()
