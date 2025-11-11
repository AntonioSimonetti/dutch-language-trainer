/*
 * UI Helper Functions
 * 
 * Reusable UI utility functions for terminal-based interface.
 * Provides functions for text positioning, drawing, and layout.
 */

#ifndef UI_H
#define UI_H

/**
 * Draw text centered horizontally at the specified row
 * 
 * @param y Row position (0 = top of screen)
 * @param text Text string to display (null-terminated)
 */
void draw_centered_text(int y, const char *text);

/**
 * Display a "Coming Soon" placeholder screen
 * 
 * Shows a centered message and waits for user to press any key
 * before returning to the previous screen.
 */
void show_placeholder(void);

#endif