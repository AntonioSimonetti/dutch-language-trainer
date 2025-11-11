/*
 * UI Helper Functions - Implementation
 * 
 * Implementation of reusable UI components for the terminal interface.
 */

#include <pdcurses.h>
#include <string.h>

void draw_centered_text(int y, const char *text){
    // Calculate horizontal center position based on terminal width
    int x = (COLS - strlen(text)) / 2;
    mvprintw(y,x, "%s", text);
}

void show_placeholder(){
    clear();
    // Display centered messages
    draw_centered_text(LINES/2 - 1, "COMING SOON");
    draw_centered_text(LINES/2, "Press any key to return to menu...");
    refresh();
    // Wait for any key press before returning
    getch();
}