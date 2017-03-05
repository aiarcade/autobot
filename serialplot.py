import serial
import time


port = serial.Serial('COM3',115200, timeout=1)


try:
    i=0
    while True:


        if i%2==0:
            port.write("F"+format(500, '05d')+"\n")
        else:
            port.write("L"+format(500, '05d')+"\n")
        i=i+1
        y=port.readline()
        time.sleep(1)
        if len(y)==0:
            print "no data"
        else:
            print y
except KeyboardInterrupt:
    port.close()
    print 'interrupted!'
