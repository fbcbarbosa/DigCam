import serial
import sys

comDict = {
    'on'    : 1,
    'off'   : 2,
    'take'  : 3
    }

def com2hex(com): # convert user command to equivalent hex code for the PIC
    try:
        x = chr(comDict[com])
    except KeyError or TypeError:
        sys.stdout.write("That was not a valid command!")
        x = chr(0)
    return x

def printList(): # print list of commands
    for k in sorted(comDict.keys()):
        print '\t' + k


ser = serial.Serial('/dev/ttyUSB1', 115200, timeout=1) # open USB port
print 'Reading from ' + ser.name
print 'Command list:'
printList()
print

while True:
    while ser.inWaiting() > 0:
        sys.stdout.write(ser.read())
    com = raw_input('Insert command: ')
    ser.write(com2hex(com))
    sys.stdout.write(ser.readline())
    
ser.close();
    

