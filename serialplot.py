import serial
import time


port = serial.Serial('COM3',115200, timeout=1)


try:
    i=100
    while True:


        if i%2==0:
            port.write("F"+format(200, '05d')+"\n")
        else:
            port.write("B"+format(200, '05d')+"\n")
        i=i+1
        print port.readline()
except KeyboardInterrupt:
    port.close()
    print 'interrupted!'
