# CAN API Tools
(This code has not yet been tested with a Hackerboard).


## Setup

## General Usage


### Compiling the files in `sample_c/libs`
Make sure that AVR is installed (run these in the command line):
```
sudo apt-get update
sudo apt-get install gcc-avr binutils-avr gdb-avr avr-libc avrdude
```
Compiling code:  
`avr-gcc shared_defs.h can_api.c can_api.h shutdown_generated.c shutdown_generated.h -o shutdown -mmcu=atmega16m1`

### TODO's
Check the TODO's in the folder to see what needs to be tested once we get access to a Hackerboard. There are a couple of design decisions regarding how to regulate and check when messages need to be sent (whether we use RTOS or just interrupts).
