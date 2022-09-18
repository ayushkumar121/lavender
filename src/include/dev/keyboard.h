#pragma once

#define KEY_EVENT_PRESSED 0
#define KEY_EVENT_RELEASED 1

void keyboard_init();
char keyboard_getch(int event);