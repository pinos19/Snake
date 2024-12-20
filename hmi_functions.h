#ifndef _HMI_FUNCTIONS_H_
#define _HMI_FUNCTIONS_H_

#include <windows.h>
#include <cstdint>

void playButton(HWND hwnd, const wchar_t textMessage[], int widthWindow, int heightWindow, int idButton);
void updatePlayButtonPosition(HWND hwnd, int idButton);
void buttonStyle(DRAWITEMSTRUCT *const pdis, const wchar_t textMessage[]);


#endif