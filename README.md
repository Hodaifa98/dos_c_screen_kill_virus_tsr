# dos_c_screen_kill_virus_tsr

## Introduction

A DOS virus that kills the screen. Written in C and is a TSR program.

This is an attempt at learning 16-bit programming on the DOS system. The purpose of this program is to act as a TSR virus that kills the screen, even after quitting the program.

Below is a short, on the surface, but concise explanation of various terms I've referenced, and code bits I've used.

**Warning:** This is done for learning purposes only.


## TSR

TSR, or Terminate and Stay Resistant programs remain active inside a computer's memory after quitting the program, which happens when it returns to the main routine. In the MS-DOS operating system, we can keep a program resident in the memory by overlaying on existing interrupts handlers.


## Interrupt

In computers, devices usually can cause interrupts when in a request for a particular service. Such as getting/receiving a byte. An example would be pressing a key, and that will cause an interrupt. 

In the x68 architecture, there are 256 different interrupts (from 0 to 255), and each number is used to perform a specific action. The first 1024 bytes of memory is reserved for the 256 far pointers, which are also called interrupt vectors. To special system routines, vector interrupt handlers are called by executing the x68 system instructions.

When an interrupt is detected in a microprocessor, the current instruction is completed and then pushed to the stack along with the address of the next instruction, which also acts as the return address. Next, the last element is popped from the stack, and then the system uses the return address to resume doing the current task before the interrupt occurred. 

We can use interrupt numbers to interact with the mouse and keyboard. To reference an interrupt number, we can check the interrupt vector table, below is a simple example containing two interrupt numbers:

| Vector Number | Vector | Cause | Origin |
| :---: | :---: | :---: | :---: |
| 0 | 0 | CPU | - |
| 8 | 020 | IRQ0 | Timer |
| 9 | 024 | IRQ1 | Keyboard |


#### IRQ

Interrupt requests (or IRQ) are hardware signals sent to the processor so that a running program can be temporarily stopped, and instead, a special program can be run instead, and that is the interrupt handler. Interrupts concerning hardware are used to handle events like receiving data from modems/network cards, keypresses in the keyboard, or even mouse movements.


## getvect() and setvect()

These are two functions I've used in this program to manipulate the interrupt vectors:

*getvect()* gets the value of one of the DOS system pre-defined interrupt vectors. The 4-byte value in each vector is an address where the interrupt function is located. It takes an integer referencing a value in the interrupt vector. The vector named in "intr_num" (the parameter) is interpreted as a far pointer to some interrupt function.

*setvect()* sets an interrupt vector entry. It sets the value of the vector provided in as a parameter (an integer) to a new value, "vector". Which should be a *far* pointer containing the address of a new interrupt function (that you can declare). To pass a C routine as the "vector", it should be declared as an "interrupt" routine.


## Far pointers

The default pointers in C are near. For example: int *p is a near pointer. And in the case of a 16-bit compiler, the size of a pointer is 2 bytes, but in general, the size varies between different compilers. They only store the offset of the pointer's address it is referencing. An address consisting of only an offset has a range of 0 - 64K bytes To access memory beyond 64K in the allocated segment, we need far pointers.

Far pointers are 4 bytes in size. They store both the segment and the offset of the address the pointer is referencing.


## Video memory

In the DOS system, the address **0xB8000000** (0xB800:0 as segment:offset) is the starting address of text video memory with 0xA8000000 being that for graphics. Any data written directly in this memory area is copied to the VGA card directly.

The starting location bytes from this address **0xB8000000** (equal to the screen's resolution * 2) are copied directly into video memory.

All characters on the screen are made up of two bytes. The first is the ASCII character, and the second is the color. Both of which must be supplied when drawing to the graphics memory.

Example of usage:
```
//Using the far pointer to access the graphics memory.
char *far screen = 0xB8000000;
//Print the character 'H' on screen at left top.
*p = 'H'; 
```
Accessing the graphics memory:
```
char far* scr = (char far*) 0xb8000000; 
```
Here is a table of the MS-DOS memory map, where we can see the location and address of the video graphcis.

![MS-DOS Memory Map](https://user-images.githubusercontent.com/40869908/94881806-3eae9d80-045e-11eb-97a0-891a1f87ad1d.JPG)

In the text mode, there are 80 columns and 25 rows, amounting to a total of 2000 characters that we can show at the same time in the screen.


## How does this virus work?

I access the graphics memory using a far pointer.
I hookup the timer interrupt by setting my own interrupt handlers to a function I've defined.
Whenever a timer interrupt gets called or the functions which depends on the timer interrupt get called, the virus's code executes at thet instant.
I use the interrupt number 8, signifying the system timer interrupt. Which is a common practice to use to "continuously" do things on a DOS machine.
It generates a random location for the row, and column where we try to 
The function writes an empty character with black color, making a square box that I use to slowly kill the screen.
I use TSR programming to keep the virus alive and running even after quitting the program. This is done using the "keep" function in the "dos.h" library. This function reserves an area of memory, by specifying how many bytes to reserve, which will not be overwritten by any other program after the virus termination.

## Usage

Compile and run the main C file in a 16 bit DOS environment (I use TURBO C++), and then run the generated executable.

## Demonstration

![Sreen Killer Demonstration](https://user-images.githubusercontent.com/40869908/94887791-1e3b0f00-046f-11eb-8a04-6121b6fd060b.gif)
