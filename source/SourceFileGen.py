#created by: Venugopal Velcheru
#date:   9/1/2016
#usage:     >python SourceFileGen.py <file_name> <file_type>
#Working:  This script creates a new C file in the directory from where this script
#          is run with name of the file to be <file_name> and type of the file
#          to be <file_type>. This file can be modified to create h file too,
#          but h file with all the header and footer info can be created from IDEs
#          like CSS or Keil. If the file already exists in the directory then it
#          prompts the user to change the file name. Also this script do not execute
#          if the python version is not 2.7.5

import os
import sys
import platform
import datetime

#Exception handling
class Error(Exception):
    "Base class for other exceptions"
    pass

class IncorrectVersion(Error):
    "Raised when the python version installed is not 2.7.x"
    pass

class FileFound(Error):
    "Raised when the file already exists in the directory"
    pass

class InvalidType(Error):
    "Raised when the file type is neither .c nor .h"

# functions
def file_create(file_name, file_type):
    fobj = open(file_name+"."+file_type, 'a+')
    print("SUCCESS")
    print(file_name+"."+file_type+" generated sucessfully at "+os.getcwd())
    return fobj

def file_append(file_obj):
    file_obj.write(file_data)
    file_obj.close()
    os.system(filename+"."+filetype)
    return

## start of code
# storing input arguments
filename = sys.argv[1] # this contains file name
filetype = "c"#sys.argv[2] can be added to create h files
# get date info
today = datetime.date.today()
# Header and footer section in the file generated
file_data = """/*                                                               
 * File name: """+ filename+"."+filetype+""" 
 *                                                                             
 *  Created by: Venugopal Velcheru
 *  Created on: """+str(today.month)+"/"+str(today.day)+"/"+str(today.year)+"""
 *  Updated on:                                                                
 */                                                                            

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives                          
//-----------------------------------------------------------------------------
#if !defined(STDINT_H_)
#include <stdint.h>
#endif
#if !defined(STDBOOL_H_)
#include <stdbool.h>
#endif
#if !defined(__TM4C123GH6PM_H__)
#include <tm4c123gh6pm.h>
#endif

#define
//-----------------------------------------------------------------------------
// Global Declarations                                                         
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Subroutines                                                                 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// End of file                                                                 
//-----------------------------------------------------------------------------"""

# storing present working directory
pwd = os.getcwd()
# getting the full file path
filepath = pwd+"\\"+filename+"."+filetype
# try block begin
while True:
    try:
        version = platform.python_version()
        if version != '2.7.5':
            raise IncorrectVersion
        if ((filetype != "c")):
            raise InvalidType
        if (os.path.isfile(filepath)):
            raise FileFound
        FileObj = file_create(filename,filetype)
        file_append(FileObj)
        break
    except IncorrectVersion:
        # exception for incorrect python version
        print("File not generated")
        print("Error: This script runs only on version 2.7.x")
        break
    except InvalidType:
        # exception for invalid file type entered
        print("File not generated")
        print("Error: Invalid file type. Please enter either c or h")
        break
    except FileFound:
        # exception if creating file already exists
        print("File not generated")
        print("Error: File already exists. Please use a different file name")
        break
