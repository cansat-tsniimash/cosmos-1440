import serial
import struct
import magic

port = serial.Serial("COM8", baudrate=115200)
port.timeout = 0.1
p = magic.Parser()

with open("file.bin", mode = "wb") as stream, open("file.csv", mode="w") as csv:
    while True:
        data = port.read(4096)
        stream.write(data)
        packets = p.parse(data)
        for packet in packets:
            numbers = magic.convert(packet)
            csv.write(";".join([str(x).replace("." , ",") for x in numbers]) + "\n")
            print(packet, numbers)
