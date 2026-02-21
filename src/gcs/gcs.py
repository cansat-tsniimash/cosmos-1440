import serial
import struct
import magic

port = serial.Serial("COM8", baudrate=115200)
port.timeout = 0.1
p = magic.Parser()

with open("file.bin", mode = "wb") as stream:
    while True:
        data = port.read(4096)
        stream.write(data)
        packets = p.parse(data)
        for packet in packets:
            print(packet)