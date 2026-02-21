import struct


class Parser:
    def __init__(self) -> None:
        self.leftowers = bytes()

    def parse(self, data: bytes):
        rv = []
        data = self.leftowers + data
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
                rv.append(walues)
                print(walues)
                data = data[27:]
            else:
                data = data[1:]
                rv.append(None)

        self.leftowers = data
        return rv