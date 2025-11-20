#include <windows.h>

void play_success_beep(void) {
    Beep(988, 70);
    Beep(1319, 140);
    Sleep(50);
    Beep(988, 70);
    Beep(1319, 140);
}

void play_error_beep(void) {
    Beep(311, 150);
    Beep(208, 300);
}
