#!/usr/bin/python3
import struct
import array
import serial
import time


BUF = bytearray(21)
count = 0

def packData(dist: float, angle: float, maxLineVel: float, maxAngleVel: float):
    struct.pack_into('<4B4f', BUF, 0, 0x52, 0x71, 0x01, 0x15, dist, angle, maxLineVel, maxAngleVel)
    checksum = 0
    for i in BUF[:-1]:
        checksum += i
        checksum = (checksum & 0xff)
    struct.pack_into('<B', BUF, 20, checksum)
    print(checksum)
    print(BUF)


def main():
    global count
    #a = 0.1
    ser1 = serial.Serial('/dev/ttyTHS1',
                          baudrate =115200,
                          bytesize=serial.EIGHTBITS,
                          parity=serial.PARITY_NONE,
                          stopbits=serial.STOPBITS_ONE)
    while True:
        packData(1.2, 1.2, 1.2,  1.2)
        print(BUF)
        count += 1
        print(count)
        print("set a package")
        time.sleep(1)


if __name__ == "__main__":
    main()

