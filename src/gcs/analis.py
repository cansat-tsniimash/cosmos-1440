import magic

p = magic.Parser()

with open("file.bin", mode = "rb") as stream:
    while True:
        data = stream.read(1024)
        packets = p.parse(data)
        for packet in packets:
            print(packet)