# Embedded-System-Software
Implement program running on Embedded Linux/Android

## Environment
Cross-Platform development  
Host PC: Ubuntu 14.04 on x64 architecture  
Target Board: Embedded Linux System on ARM architecture

1. Project 1  
Implement Clock, Counter, Text editor, Draw board  
using device control, IPC

2. Project 2  
Implement Timer device driver

## My tips for C language
### Use structure for parameter
Scalability: can add other parameters later without fixing function prototype

### Hide variables
Tried to simulate encapsulation in C.
Declare variables as static global at C file, not at header file if not used at other files.

### Set function name conveniently
Add file name as prefix for function name. That way, one can find function definition conveniently.