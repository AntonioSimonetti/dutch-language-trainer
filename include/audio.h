/*
 * Audio Feedback Module
 * 
 * Provides sound effects for user feedback.
 */

#ifndef AUDIO_H
#define AUDIO_H

/**
 * Play a success sound (correct answer)
 * 
 * a correct answer or successful action.
 */
void play_success_beep(void);

/**
 * Play an error sound (wrong answer)
 * 
 * an incorrect answer or error.
 */
void play_error_beep(void);

#endif