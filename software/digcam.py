#!/usr/bin/env python
#!/usr/bin/python -i

import numpy as np
from PIL import Image

import serial
import sys
import os

# user imports
import instructions

# global variables
ser = 0
version = 0.1
WIDTH = 800
HEIGHT = 200

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
        
        if com == 'close':  
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
            print '\t Roberto Walter'
            
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
                    
                answer = ser.read()         # check answer
 
                # print until end of stream             
                while answer != END:        # END - code for end of stream
                    if answer == LOG:
                        readImage()
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
   
def readImage():
    """
    Read the photo.
    """
    error = 0
    imgarray = np.empty([HEIGHT,WIDTH])
    for i in range(0, HEIGHT):
        for j in range(0, WIDTH):
            answer = ser.read()
            if answer != '': 
                imgarray[i][j] = int(ord(answer)) # write ASCII code (integer value) of char    
            else: # timeout
                error = error + 1
                j = j - 1 # try again to get a char!
            print '\rByte count = {0}/{1}\tTimeout count = {2}'.format(WIDTH*i + j + 1, WIDTH*HEIGHT, error),
     
    img = Image.fromarray(imgarray)
    img.show()
    img.mode = "L"
    img.save(os.path.join(os.pardir, "temp\\photo.bmp"), "L")
    print 'Image saved in "'"DigCam\\temp\\.photo.bmp"'"'

    txt = open(os.path.join(os.pardir, "temp\\grayscale.txt"), "w+")
    for i in range(0, HEIGHT):
        for j in range(0, WIDTH):
            txt.write(str(int(imgarray[i][j])) + " ") # write ASCII code (integer value) of char
    print 'Grayscale matrix saved in "'"DigCam\\temp\\grayscale.txt"'"'
             
def helpMessage():
    """
    Display help message.
    """
    global ser # serial communication handler
    print 'Reading from ' + ser.name
    print 'Terminal command list:'
    print '\t' +  'about' + '\t\t' + 'Display program info'
    print '\t' +  'close' + '\t\t' + 'Close program'
    print '\t' +  'help'  + '\t\t' + 'Display the instruction list'
    print 'Camera command list:'
    instructions.initDatabase()
    instructions.listAll()
    print 'IMPORTANT: parameters must be decimal values between 0 and 255.'
    
        
if __name__ == "__main__":
    main()
