import serial
import struct
port = serial.Serial("COM8", baudrate=115200)
port.timeout = 0.1

data = bytes()
while True:
    data += port.read(4096)
    while len(data) >= 27:
        while len(data) > 2 and (data[0] != 0xaa or data[1] != 0xaa):
            data = data[1:]
        
        if len(data) < 27:
            break

        packet = data[2:27]
        cchk = packet[0]
        for b in packet[1:]:
            cchk = cchk ^ b

        if cchk == 0:
            walues = struct.unpack("<HIhI3h3hB", packet)
            print(walues)
            data = data[27:]
        else:
            data = data[1:]
            print("Bad")
