//Including the "dos.h" library to use functions for handling interrupts...
#include"dos.h"
//Used by MS-DOS compilers to provide console input/output.
#include<conio.h>
//General purpose standard library of C.
#include<stdlib.h>


//Declaring functions prototypes.

//Pointer to a function .
//The "interrupt" keyword means that it's an interrupt handler.
void interrupt (*prevtimer)();
void interrupt screen_killer_timer();

//This function will write a character to a position in the graphcis screen.
void write_character_to_screen(char character, int row, int column, int color);


//Global variables.

//A far pointer to the screen graphics memory.
char far* screen_graphics;

//Variables to hold the random values of screen coordinates.
int a,b;
int random_color, char_to_write;


//Main program.
void main() {

    //Access graphics/video memory.
    screen_graphics = (char far*) 0xb8000000;

    //Receive the address from the interrupt vector table.
    //8 is the interrupt number for the timer.
    prevtimer = getvect(8);

    //Setting the interrupt vector entry to our own defined interrupt handler.
    //Whenever the interrupt number 8 occurs or gets called, our function executes.
    setvect(8, screen_killer_timer);

    //We reserve 1000 bytes of space memory to run our program even after exiting.
    //Unlike normal completion of main, where DOS marks all RAM occupied before by the program as free.
    //The program will exit with code 0 (success), then tells DOS to leave 1000 bytes in the RAM.
    keep(0,1000);
}

//screen_killer_timer.
void interrupt screen_killer_timer() {
    //Randomly get integers between 0 and 25 and assign them to a and b.
    a = random(25);
    b = random(80);


    /* To have fun colors, use this instead of the code below.
    char_to_write = 0;
    random_color = random(15);
    char_to_write = random(2) + '0';
    write_character_to_screen(char_to_write,a,b,random_color);
    (*prevtimer)();
    */

    //Writing a char (empty) to the a and b which represent the column and row.
    //The random color is black (represented by 0).
    write_character_to_screen(' ', a, b, 0);
    //Calling the previous and original timer interrupt.
    (*prevtimer)();

}

//write_character_to_screen.
void write_character_to_screen(char character, int row, int column, int color) {
    //Writing a single character to the computer screen.
    //In this case, it's a black square.
    //The first byte is the character.
    *(screen_graphics + row * 160 + column * 2) = character;
    //The next byte (adjacent) is the color byte.
    *(screen_graphics + row * 160 + column * 2 + 1) = color;

}
