#!/usr/bin/env python

import sys

"""
This module implements a instruction database.
"""
class Instruction:
    """
    This class implements a instruction.
    """
    
    def __init__(self, code, name, description):
        """
        Creates a new instruction.
        
        PARAMETERS
        code - char: microcontroller's instruction code
        name - string: call name of the instruction
        description - string: description of the instruction
        """
        self.code = code;
        self.name = name;
        self.description = description;

__database = {}

def initDatabase():
    add(Instruction(chr(1), 'on',  'Turns on the camera.'))
    add(Instruction(chr(2), 'off', 'Turns off the camera.'))
    add(Instruction(chr(3), 'take', 'Take a picture.'))
    add(Instruction(chr(4), 'get p', 'Get parameter p value'))
    add(Instruction(chr(5), 'set p', 'Set parameter p value.'))
    
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
        print '\t' + k + '\t' + __database[k][1]
        
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
        if n == 1:   # its a single-word command, like 'on' or 'off'
            ch1 = __database[command][0]
        elif n == 2: # its a two-word command, like 'get color'
            ch1 = __database[words[0] + " " + words[1]][0]
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