/*
 * Language Selection Screen
 * 
 * Displays a menu for selecting user's native language.
 * Only Italian is currently available, other languages show "Coming Soon".
 */

#ifndef LANGUAGE_SELECTION_H
#define LANGUAGE_SELECTION_H

/**
 * Display language selection screen and get user's choice
 * 
 * Shows a menu with available languages. Currently only Italian (IT)
 * is fully implemented. Other languages show "Coming Soon" message.
 * 
 * @param language_out Buffer to store selected language code (min 3 chars: "IT\0")
 */
void show_language_selection(char *language_out);

#endif