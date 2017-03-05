import serial
import time


port = serial.Serial('COM3',115200, timeout=1)


try:
    i=100
    while True:
        print port.readline()
        time.sleep(1)
        port.write("F"+format(i, '05d')+"\n")
        i=i+1
except KeyboardInterrupt:
    port.close()
    print 'interrupted!'
