#!/usr/bin/env python

import time
import os
import serial
import sys

# user imports
import instructions

# global variables
ser = 0
version = 0.1;

# constants
LOG = chr(253)
ACK = chr(254)
END = chr(255)

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
    
    # deletes trash       
    while ser.inWaiting() > 0: 
        ser.read() 
         
    # main loop
    while True:
        
        com = raw_input('> Insert instruction: ')   # ask user for command
        
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
            
            if code != -1:                  # check if there was an error
                ser.write(code)             # write serial data
                
                # wait for acknowledgement
                answer = ser.read()         # check answer
                while answer != ACK:        # ACK - code for acknowledgement
                    answer = ser.read()
                    
                    if answer == '':
                        print 'Error: connection timeout!'
                        sys.exit()
                
                # print until end of stream
                answer = ser.read()         # check answer
                             
                while answer != END:        # END - code for end of stream
                    if answer == LOG:
                        startLogging()
                    else:
                        sys.stdout.write(answer)
                    
                    answer = ser.read()
                    
            
def openRoutine():
    """
    Tries to find and open the correct serial port.
    """
    global ser # serial communication handler 
    
    while True:
        port = raw_input('> Insert COM/USB port number ("'"exit"'" to quit): ');
        if port == 'exit':
            return -1;
        
        try:
            ser = serial.Serial(int(port), 115200, timeout=3)   # Windows
        except serial.serialutil.SerialException:
            try:
                ser = serial.Serial('/dev/ttyUSB' + port, 115200, timeout=3)    # Linux
            except serial.serialutil.SerialException:
                print 'Error: did not find PicDev! Check your USB connection or try another port.'
            else:
                return 0
        else:
            return 0          
    
def helpMessage():
    """
    Display help message.
    """
    global ser # serial communication handler
    print 'Reading from ' + ser.name
    print 'Terminal command list:'
    print '\t' +  'about' + '\t\t' + 'display program info'
    print '\t' +  'close' + '\t\t' + 'close program'
    print '\t' +  'exit'  + '\t\t' + 'close program'
    print '\t' +  'help'  + '\t\t' + 'display the instruction list'
    print '\t' +  'quit'  + '\t\t' + 'close program'
    print 'Camera command list:'
    instructions.initDatabase()
    instructions.listAll()
    print 'IMPORTANT: parameters must be decimal values between 0 and 255.'

def startLogging():
    """
    Logs UART output to a file.
    """
    log = open('log.txt', 'w+')
    
    i = 0;
    while True:
        i = i + 1
        answer = ser.read()
        
        if answer == '':
            print '\nLog saved at log.txt!'
            log.close()
            return
        
        print '\rByte Count = {0}'.format(i),
        log.write(answer)
    
if __name__ == "__main__":
    main()