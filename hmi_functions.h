#ifndef _HMI_FUNCTIONS_H_
#define _HMI_FUNCTIONS_H_

#include <windows.h>

void createPlayButton(HWND hwnd, int widthWindow, int heightWindow, int idButton);
void updatePlayButtonPosition(HWND hwnd, int idButton);
void playButtonStyle(DRAWITEMSTRUCT *const pdis);


#endif