#!/usr/bin/env python

import serial
import sys

# user imports
import instructions

# global variables
ser = 0
version = 0.1;

def main():
    """
    Run serial communication.
    """
    global ser # serial communication handler
    global version
    
    print 'Welcome to DigCam Interface!'
    
    # if can't open USB port, quit
    if openRoutine() == -1:
        sys.exit()           
        
    # display instructions
    helpMessage()             
    
    # main loop
    while True:
        while ser.inWaiting() > 0:
            sys.stdout.write(ser.read())          # read serial data
        
        com = raw_input('> Insert instruction: ') # ask user for command
        
        if com == 'quit' or com == 'exit' or com == 'close':  
            ser.close();
            print 'Goodbye!'
            break
        elif com == 'help':
            helpMessage()
        elif com == 'about':
            print 'DigCam Interface Version ' + str(version)
            print 'Authors:'
            print '\t Fernando Barbosa'
            print '\t Joao Lucas Kunzel'
            print '\t Roberto "El Loco" Walter'
        else: # then it wasn't a terminal command
            code = instructions.encode(com);
            if code != -1:       # check if there was an error
                ser.write(code)  # write serial data
                sys.stdout.write(ser.readline())

def openRoutine():
    """
    Tries to find and open the correct serial port (search USB0, USB1 and USB2).
    """
    global ser # serial communication handler 
    try:
        ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
    except serial.serialutil.SerialException:
        try:
            ser = serial.Serial('/dev/ttyUSB1', 115200, timeout=1)
        except serial.serialutil.SerialException:
            try:
                ser = serial.Serial('/dev/ttyUSB2', 115200, timeout=1)
            except serial.serialutil.SerialException:
                print 'Error: did not find PicDev! Please check your USB connection.'
                return -1
    return 0
    
def helpMessage():
    """
    Display help message.
    """
    global ser # serial communication handler
    print 'Reading from ' + ser.name
    print 'Terminal command list:'
    print '\t' +  'about' + '\t' + 'display program info'
    print '\t' +  'close' + '\t' + 'close program'
    print '\t' +  'exit'  + '\t' + 'close program'
    print '\t' +  'help'  + '\t' + 'display the instruction list'
    print '\t' +  'quit'  + '\t' + 'close program'
    print 'Camera command list:'
    instructions.initDatabase()
    instructions.listAll()

    
if __name__ == "__main__":
    main()