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
        
def lookup(name):
    """
    Lookup instruction in the database and return its code.

    Keyword arguments:
    name - string: the instruction name
    """
    try:
        x = __database[name][0]
    except KeyError or TypeError:
        print name + " is not a valid command!"
        x = chr(0)
    return x
