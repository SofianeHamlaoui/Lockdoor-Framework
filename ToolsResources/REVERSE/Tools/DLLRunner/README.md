DLLRunner
=========

DLLRunner is a smart DLL execution script for malware analysis in sandbox systems. 

Instead of executing a DLL file via "rundll32.exe file.dll" it analyzes the PE and executes all exported functions by name or ordinal in order to determine if one of the functions causes malicious activity.

    rundll32.exe path/to/file.dll,exportedfunc1
    rundll32.exe path/to/file.dll,exportedfunc2
    rundll32.exe path/to/file.dll,exportedfunc3

Furthermore it tries to fuzz parameters in order to trigger acitivity in functions that require parameters to work. 

    rundll32.exe path/to/file.dll,exportedfunc1 "0"
    rundll32.exe path/to/file.dll,exportedfunc1 "1"
    rundll32.exe path/to/file.dll,exportedfunc1 "http://evil.local"
    rundll32.exe path/to/file.dll,exportedfunc1 "Install" 
    ...

Usage
=========

    usage: dllrunner.py [-h] [-f dllfile] [-l limit] [--fuzz] [--demo] [--debug]
    
    DLLRunner
    
    optional arguments:
      -h, --help  show this help message and exit
      -f dllfile  DLL file to execute exported functions
      -l limit    Only perform extended calls if export function count is less
                  than limit
      --fuzz      Add fuzzing parameters to the functions calls (currently 5
                  params are defined)
      --demo      Run a demo using \system32\url.dll
      --debug     Debug output
