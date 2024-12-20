#include "hmi_functions.h"

void updatePlayButtonPosition(HWND hwnd, int idButton){
    RECT rect;
    GetClientRect(hwnd, &rect);

    int widthWindow = rect.right - rect.left;
    int heightWindow = rect.bottom - rect.top;

    int widthButton = widthWindow/14.24;
    int heightButton = heightWindow/14.12;

    int xStartingPoint = (widthWindow - widthButton)/2;
    int yStartingPoint = (heightWindow - heightButton)/2;

    HWND hButton = GetDlgItem(hwnd, idButton);
    if( hButton ){
        SetWindowPos(hButton, nullptr, xStartingPoint, yStartingPoint, widthButton, heightButton, SWP_NOZORDER);
    }   
}
void playButton(HWND hwnd, const wchar_t textMessage[], int widthWindow, int heightWindow, int idButton){
    int widthButton = widthWindow/14.24;
    int heightButton = heightWindow/14.12;

    int xStartingPoint = (widthWindow - widthButton)/2;
    int yStartingPoint = (heightWindow - heightButton)/2;

    void* ptr = reinterpret_cast<void*>(static_cast<std::uintptr_t>(idButton));

    CreateWindow(
        L"BUTTON", textMessage,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        xStartingPoint, yStartingPoint, widthButton, heightButton,
        hwnd,(HMENU)ptr,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        nullptr);
}
void buttonStyle(DRAWITEMSTRUCT *const pdis, const wchar_t textMessage[]){
    // Fonction qui donne le style pour le bouton Jouer

    HDC hdc = pdis->hDC;
    RECT rect = pdis->rcItem;

    boolean isPressed = (pdis->itemState & ODS_SELECTED);

    COLORREF backgroundPressed = RGB(0,0,0);
    COLORREF backgroundNotPressed = RGB(150,150,150);
    COLORREF textPressed = RGB(150,150,150);
    COLORREF texNotPressed = RGB(0,0,0);

    HBRUSH hbrush;

    // Création de la police
    HFONT hFont = CreateFont(
        20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, // Taille et style
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial"); // Famille "Arial"

    if( isPressed ){
        // Le bouton est sélectionné
        hbrush = CreateSolidBrush(backgroundPressed);
        SetTextColor(hdc, textPressed);
    }else{
        // Le bouton n'est pas sélectionné
        hbrush = CreateSolidBrush(backgroundNotPressed);
        SetTextColor(hdc, texNotPressed);
    }
    FillRect(hdc, &rect, hbrush);
    DeleteObject(hbrush);

    // Mettre le fond transparent pour le texte
    SetBkMode(hdc, TRANSPARENT);

    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    DrawText(hdc, textMessage, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Restaurer la police d'origine
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}