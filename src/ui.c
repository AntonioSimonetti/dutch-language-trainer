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

void draw_box(int x, int y, int width, int height) {
    // Top border
    mvaddch(y, x, ACS_ULCORNER);
    for (int i = 1; i < width - 1; i++) {
        mvaddch(y, x + i, ACS_HLINE);
    }
    mvaddch(y, x + width - 1, ACS_URCORNER);
    
    // Sides
    for (int i = 1; i < height - 1; i++) {
        mvaddch(y + i, x, ACS_VLINE);
        mvaddch(y + i, x + width - 1, ACS_VLINE);
    }
    
    // Bottom border
    mvaddch(y + height - 1, x, ACS_LLCORNER);
    for (int i = 1; i < width - 1; i++) {
        mvaddch(y + height - 1, x + i, ACS_HLINE);
    }
    mvaddch(y + height - 1, x + width - 1, ACS_LRCORNER);
}