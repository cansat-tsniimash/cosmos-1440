import magic
import csv

parser = magic.Parser()

with open("file.bin", "rb") as bin_file, open("file.csv", "w", newline="", encoding="utf-8") as csv_file:

    writer = csv.writer(csv_file, delimiter=" ")

    while True:
        data = bin_file.read(4096)
        if not data:
            break

        packets = parser.parse(data)

        for packet in packets:
            values = magic.convert(packet)
            writer.writerow(values)

print("УСПЕХ")