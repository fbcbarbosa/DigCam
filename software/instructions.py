#!/usr/bin/env python

import sys

"""
This module implements a instruction database.
"""
class Instruction:
    """
    This class implements a instruction.
    """
    
    def __init__(self, number, name, description):
        """
        Creates a new instruction.
        
        PARAMETERS
        number - int: an integer between 0 and 255
        name - string: call name of the instruction
        description - string: description of the instruction
        """
        self.code = chr(number);
        self.name = name;
        self.description = description;

__database = {}

def initDatabase():
    add(Instruction(1, 'on',  'Turns on the camera'))
    add(Instruction(2, 'off', 'Turns off the camera'))
    add(Instruction(3, 'photo', 'Take a picture'))
    add(Instruction(4, 'reset', 'Executes software reset'))
    add(Instruction(5, 'status', 'Prints camera status'))
    add(Instruction(6, 'read0', 'Reads register table 0xFF = 0'))
    add(Instruction(7, 'read1', 'Reads register table 0xFF = 1'))
    
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
    ch1 = chr(0);    # PIC instruction code
    
    try:
        if n == 0:
            print "Error: '" + command + "'" + " is not a valid command!"
            return -1
        elif n == 1:
            ch1 = __database[command][0]         
        else:
            raise KeyError
    except KeyError:
        print "Error: '" + command + "'" + " is not a valid command!"
        return -1
    return ch1
