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
                data = data[27:]
            else:
                data = data[1:]
                rv.append(None)

        self.leftowers = data
        return rv
    

def convert(valeus: tuple):
    return(
    valeus[0],#time id
    valeus[1],#time
    valeus[2]/16, #temperature
    valeus[3], #давление
    lsm6ds3_from_fs16g_to_mg(valeus[4]),
    lsm6ds3_from_fs16g_to_mg(valeus[5]),
    lsm6ds3_from_fs16g_to_mg(valeus[6]),
    lsm6ds3_from_fs2000dps_to_mdps(valeus[7]),
    lsm6ds3_from_fs2000dps_to_mdps(valeus[8]),
    lsm6ds3_from_fs2000dps_to_mdps(valeus[9]),
)


def lsm6ds3_from_fs2g_to_mg(lsb: int) -> float:
  return lsb * 61.0 / 1000.0

def lsm6ds3_from_fs4g_to_mg(lsb: int) -> float:
  return lsb * 122 / 1000.0

def lsm6ds3_from_fs8g_to_mg(lsb: int) -> float:
  return lsb * 244.0 / 1000.0

def lsm6ds3_from_fs16g_to_mg(lsb: int) -> float:
  return lsb * 488.0 / 1000.0

def lsm6ds3_from_fs125dps_to_mdps(lsb: int) -> float:
  return lsb * 4375.0 / 1000.0

def lsm6ds3_from_fs250dps_to_mdps(lsb: int) -> float:
  return lsb * 8750.0 / 1000.0

def lsm6ds3_from_fs500dps_to_mdps(lsb: int) -> float:
  return lsb * 1750.0 / 1000.0

def lsm6ds3_from_fs1000dps_to_mdps(lsb: int) -> float:
  return lsb * 35.0

def lsm6ds3_from_fs2000dps_to_mdps(lsb: int) -> float:
  return lsb * 70.0

