#!/usr/bin/env python

import sys

"""
This module implements a instruction database.
"""
class Instruction:
    """
    This class implements a instruction.
    """
    
    def __init__(self, number, name, description, usage = 'name'):
        """
        Creates a new instruction.
        
        PARAMETERS
        number - int: an integer between 0 and 256
        name - string: call name of the instruction
        description - string: description of the instruction
        usage - string: call name of the instructions + parameters (if not set, equals parameter 'name')
        """
        self.code = chr(number);
        self.name = name;
        self.description = description;

__database = {}

def initDatabase():
    add(Instruction(1, 'on',  'Turns on the camera'))
    add(Instruction(2, 'off', 'Turns off the camera'))
    add(Instruction(3, 'photo', 'Take a picture'))
    add(Instruction(4, 'get p', 'Get parameter p'))
    add(Instruction(5, 'set p', 'Set parameter p', 'set p [value]'))
    add(Instruction(6, 'reset', 'Executes software reset'))
    add(Instruction(7, 'status', 'Prints camera status'))
    add(Instruction(8, 'read', 'Reads register', 'read [register]'))
    add(Instruction(9, 'write', 'Reads register', 'write [register] [data]'))
    add(Instruction(10, 'readall', 'Reads all registers'))
    add(Instruction(11, 'bmp', 'Creates test BMP file'))
            
def add(instruction):
    """
    Adds a new instruction to the database.
    """
    __database[instruction.name] = [instruction.code, instruction.description]
      
def listAll():
    """
    Lists all instructions and their descriptions.
    """
    for k in sorted(__database.keys()):
        print '\t' + k + '\t\t' + __database[k][1]
        
def encode(command):
    """
    Interpret the command according to the Instruction Database and return its code. 
    Handles errors.

    Keyword arguments:
    command - string: the command given
    """
    words = command.split() # split the command in single words
    n = len(words)
    ch1 = chr(0);    # first char, PIC instruction code
    ch2 = chr(0);    # second char, instruction parameter (optional)
    try:
        if n == 0:
            print "Error: '" + command + "'" + " is not a valid command!"
            return -1
        elif n == 1:   # its a single-word command, like 'on' or 'off'
            ch1 = __database[command][0]
        elif n == 2 and words[0] == 'read': # read register command
            try:
                param = int(words[1]); # parameter
                if param < 0 or param > 255:
                    raise ValueError
                ch1 = __database[words[0]][0]
                ch2 = chr(param)
            except ValueError:
                print "Error: '" + words[2] + "'" + " is not and integer between 0 and 255!"
                return -1            
        elif n == 2 and words[0]: # its a two-word command, like 'get color'
            ch1 = __database[words[0] + " " + words[1]][0]
        elif n == 3 and words[0] == 'write': # write register command
            return -1 # not yet implemented
        elif n == 3: # its a three-word command, like 'set color 100'
            try:
                param = int(words[2]); # parameter
                if param < 0 or param > 255:
                    raise ValueError
                ch1 = __database[words[0] + " " + words[1]][0]
                ch2 = chr(param)
            except ValueError:
                print "Error: '" + words[2] + "'" + " is not and integer between 0 and 255!"
                return -1
        else:
            raise KeyError
    except KeyError:
        print "Error: '" + command + "'" + " is not a valid command!"
        return -1
    return ch1 + ch2