import serial
import struct
import magic
import pandas as pd
import matplotlib.pyplot as plt

port = serial.Serial("COM7", baudrate=115200)
port.timeout = 0.1
p = magic.Parser()

def calibrate_magnetometer(mx_raw, my_raw, mz_raw):
    bx = -486.860372
    by = 713.164669
    bz = 178.162411

    a11 = 0.003541
    a12 = 0.000082
    a13 = 0.000156

    a21 = 0.000082
    a22 = 0.003631
    a23 = 0.000045

    a31 = 0.000156
    a32 = 0.000045
    a33 = 0.002797

    x = mx_raw - bx
    y = my_raw - by
    z = mz_raw - bz

    x1 = a11 * x + a12 * y + a13 * z
    y1 = a21 * x + a22 * y + a23 * z
    z1 = a31 * x + a32 * y + a33 * z

    return [x1, y1, z1]

x = []
y = []
z = []
plt.ion()
fig = plt.figure()
ax = fig.add_subplot(projection="3d")
#ax.set_xlim(-600, 600)
#ax.set_ylim(-600, 600)
#ax.set_zlim(-600, 600)
plot, = ax.plot(x, y, z, linestyle='None',marker='o')
#plt.autoscale(enable=True, axis='both', tight=True)

minx = 0
maxx = 0
miny = 0
maxy = 0
minz = 0
maxz = 0
a = 0

with open("file.bin", mode = "wb") as stream, open("magneto.txt", mode="w") as csv:
    while True:
        data = port.read(4096)
        stream.write(data)
        packets = p.parse(data)
        for packet in packets:
            numbers = magic.convert(packet)
            csv.write(";".join([str(x).replace("." , ",") for x in packet[20:23]]) + "\n")
            #print(packet, numbers)
            #print(numbers)
            #print(packet[24:28])
            #print(packet[20:23])
            print(calibrate_magnetometer(*packet[20:23]))
            x.append(calibrate_magnetometer(*packet[20:23])[0])
            y.append(calibrate_magnetometer(*packet[20:23])[1])
            z.append(calibrate_magnetometer(*packet[20:23])[2])
            a += 1
            if a > 30:
                a = 0
            else:
                continue
            plot.set_xdata(x)
            plot.set_ydata(y)
            plot.set_3d_properties(z)
            minx = min(x[-1], minx)
            miny = min(y[-1], miny)
            minz = min(z[-1], minz)
            maxx = max(x[-1], maxx)
            maxy = max(y[-1], maxy)
            maxz = max(z[-1], maxz)
            ax.set_xlim(minx, maxx)
            ax.set_ylim(miny, maxy)
            ax.set_zlim(minz, maxz)
            fig.canvas.draw()
            fig.canvas.flush_events()
            plt.pause(0.1)