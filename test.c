#include <pdcurses.h>
#include <locale.h>

int main() {
    // Abilita UTF-8
    setlocale(LC_ALL, "");
    
    // Inizializza PDCurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    // Abilita colori
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    
    // Disegna interfaccia
    attron(COLOR_PAIR(1));
    mvprintw(2, 5, "╔════════════════════════════════════╗");
    mvprintw(3, 5, "║                                    ║");
    attroff(COLOR_PAIR(1));
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(3, 10, "DUTCH LANGUAGE TRAINER");
    attroff(COLOR_PAIR(2) | A_BOLD);
    
    attron(COLOR_PAIR(1));
    mvprintw(4, 5, "║                                    ║");
    mvprintw(5, 5, "╚════════════════════════════════════╝");
    attroff(COLOR_PAIR(1));
    
    attron(COLOR_PAIR(3));
    mvprintw(7, 5, "Test UTF-8: café, coördinatie, ë");
    mvprintw(9, 5, "✓ PDCurses funziona!");
    mvprintw(10, 5, "✓ UTF-8 funziona!");
    mvprintw(11, 5, "✓ Colori funzionano!");
    attroff(COLOR_PAIR(3));
    
    mvprintw(13, 5, "Premi un tasto per uscire...");
    
    refresh();
    getch();
    
    endwin();
    
    return 0;
}