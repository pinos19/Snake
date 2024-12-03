#include <windows.h>
#include "Snake.h"
#include "Grid.h"
#include "Game.h"
#include "hmi_functions.h"

#define BUTTON_PLAY_ID 1
#define MOVE_TIMER_ID 2

#define WINDOW_RESIZED 3
#define INIT_GRID_RECT 4

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_TIMER:{
            if (wParam == MOVE_TIMER_ID) {
                RECT rectWindow;
                GetClientRect(hwnd, &rectWindow);

                int rectColumnOld = rect_column.at(0);
                int rectLineOld = rect_line.at(0);

                if( is_moving_right ) {
                    RECT rectOld = rect;

                    rect.left += cell_width;
                    rect.right += cell_width;

                    InvalidateRect(hwnd, &rectOld, TRUE);

                    // On actualise la queue du serpent
                    for(int i =0; i<length_snake; i++){
                        int column_temp = rect_column.at(i+1);
                        int line_temp = rect_line.at(i+1);  
                        rect_column.at(i+1) = rectColumnOld;
                        rect_line.at(i+1) = rectLineOld;
                        rectColumnOld = column_temp;
                        rectLineOld = line_temp;

                        RECT rect_temp;
                        rect_temp = snake_tail.at(i);
                        snake_tail.at(i) = rectOld;
                        rectOld = rect_temp;    
                        InvalidateRect(hwnd, &rect_temp, TRUE);
                    }

                    if( rect.right > rectWindow.right ){
                        // On dépasse à droite, on repasse à gauche
                        rect = {offsetX+2, offsetY + (rect_line.at(0)-1)*cell_height+2, offsetX + cell_width -1, offsetY + rect_line.at(0)*cell_height - 1};
                        rect_column.at(0) = 1;
                    }else{
                        rect_column.at(0) +=1;
                    }
                    
                    InvalidateRect(hwnd, &rect, TRUE);
                    REPAINT_NEEDED = RECT_MOVING;
                }
                if( is_moving_left ){
                    RECT rectOld = rect;

                    rect.left -= cell_width;
                    rect.right -= cell_width;

                    InvalidateRect(hwnd, &rectOld, TRUE);
                    // On actualise la queue du serpent
                    for(int i =0; i<length_snake; i++){
                        int column_temp = rect_column.at(i+1);
                        int line_temp = rect_line.at(i+1);  
                        rect_column.at(i+1) = rectColumnOld;
                        rect_line.at(i+1) = rectLineOld;
                        rectColumnOld = column_temp;
                        rectLineOld = line_temp;

                        RECT rect_temp;
                        rect_temp = snake_tail.at(i);
                        snake_tail.at(i) = rectOld;
                        rectOld = rect_temp;
                        InvalidateRect(hwnd, &rect_temp, TRUE);
                    }

                    if( rect.left < rectWindow.left ){
                        // On dépasse à gauche, on repasse à droite
                        rect = {offsetX + (number_columns-1)*cell_width +2, offsetY + (rect_line.at(0)-1)*cell_height+2, offsetX + number_columns*cell_width -1, offsetY + rect_line.at(0)*cell_height - 1};
                        rect_column.at(0) = number_columns;
                    }else{
                        rect_column.at(0) -= 1;
                    }

                    InvalidateRect(hwnd, &rect, TRUE);
                    REPAINT_NEEDED = RECT_MOVING;
                }
                if( is_moving_up ){
                    RECT rectOld = rect;

                    rect.top -= cell_height;
                    rect.bottom -= cell_height;

                    InvalidateRect(hwnd, &rectOld, TRUE);
                    // On actualise la queue du serpent
                    for(int i =0; i<length_snake; i++){
                        int column_temp = rect_column.at(i+1);
                        int line_temp = rect_line.at(i+1);  
                        rect_column.at(i+1) = rectColumnOld;
                        rect_line.at(i+1) = rectLineOld;
                        rectColumnOld = column_temp;
                        rectLineOld = line_temp;

                        RECT rect_temp;
                        rect_temp = snake_tail.at(i);
                        snake_tail.at(i) = rectOld;
                        rectOld = rect_temp;
                        InvalidateRect(hwnd, &rect_temp, TRUE);
                    }

                    if( rect.top < rectWindow.top ){
                        // On dépasse à gauche, on repasse à droite
                        rect = {offsetX + (rect_column.at(0)-1)*cell_width +2, offsetY + (number_lines-1)*cell_height+2, offsetX + rect_column.at(0)*cell_width -1, offsetY + number_lines*cell_height - 1};
                        rect_line.at(0) = number_lines;
                    }else{
                        rect_line.at(0) -= 1;
                    }

                    InvalidateRect(hwnd, &rect, TRUE);
                    REPAINT_NEEDED = RECT_MOVING;
                }
                if( is_moving_down ){
                    RECT rectOld = rect;

                    rect.top += cell_height;
                    rect.bottom += cell_height;

                    InvalidateRect(hwnd, &rectOld, TRUE);
                    for(int i =0; i<length_snake; i++){
                        int column_temp = rect_column.at(i+1);
                        int line_temp = rect_line.at(i+1);  
                        rect_column.at(i+1) = rectColumnOld;
                        rect_line.at(i+1) = rectLineOld;
                        rectColumnOld = column_temp;
                        rectLineOld = line_temp;

                        RECT rect_temp;
                        rect_temp = snake_tail.at(i);
                        snake_tail.at(i) = rectOld;
                        rectOld = rect_temp;
                        InvalidateRect(hwnd, &rect_temp, TRUE);
                    }

                    if( rect.bottom > rectWindow.bottom ){
                        // On dépasse à gauche, on repasse à droite
                        rect = {offsetX + (rect_column.at(0)-1)*cell_width +2, offsetY +2, offsetX + rect_column.at(0)*cell_width -1, offsetY + cell_height - 1};
                        rect_line.at(0) = 1;
                    }else{
                        rect_line.at(0) += 1;
                    }

                    InvalidateRect(hwnd, &rect, TRUE);
                    REPAINT_NEEDED = RECT_MOVING;
                }
            }
        }
        case WM_KEYDOWN:{
            if( wParam == VK_RIGHT || wParam == VK_LEFT || wParam == VK_UP || wParam == VK_DOWN ){
                if( !timer_set ){
                    SetTimer(hwnd, MOVE_TIMER_ID, SPEED_RECTANGLE, nullptr);
                    timer_set = true;
                }
            }
            switch (wParam){
                case VK_RETURN:{
                    HWND hButton = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                    if( hButton ){
                        DestroyWindow(hButton);
                    }
                    if( !grid_set ){
                        InvalidateRect(hwnd, nullptr, TRUE);
                        REPAINT_NEEDED = INIT_GRID_RECT;
                    }else{
                        SetTimer(hwnd, MOVE_TIMER_ID, SPEED_RECTANGLE, nullptr);
                        timer_set = true;
                        InvalidateRect(hwnd, nullptr, TRUE);
                        REPAINT_NEEDED = CONTINUE_GAME;
                    }
                }
                case VK_RIGHT:{
                    if( !is_moving_left ){
                        // Se déplace autre que à gauche
                        is_moving_right =true;
                        is_moving_left = false;
                        is_moving_up = false;
                        is_moving_down = false;
                    }
                    break;
                }
                case VK_LEFT:{
                    if( !is_moving_right ){
                        is_moving_left =true;
                        is_moving_right = false;
                        is_moving_up = false;
                        is_moving_down = false;
                    }
                    break;
                }
                case VK_UP:{
                    if( !is_moving_down ){
                        is_moving_up =true;
                        is_moving_left = false;
                        is_moving_right = false;
                        is_moving_down = false;
                    }
                    break;
                }
                case VK_DOWN:{
                    if( !is_moving_up ){
                        is_moving_down =true;
                        is_moving_left = false;
                        is_moving_right = false;
                        is_moving_up = false;
                    }
                    break;
                }
                case VK_ESCAPE:{
                    if( timer_set ){
                        KillTimer(hwnd, MOVE_TIMER_ID);
                        timer_set = false;
                    }

                    // Marquer toute la fenêtre comme invalide pour déclencher un WM_PAINT
                    InvalidateRect(hwnd, nullptr, TRUE);
                    if( initialized ){
                        REPAINT_NEEDED = WINDOW_RESIZED;
                    }
                    break;
                }  
                default:
                    break;
            }
            break;
        }
        case WM_COMMAND:{
            // On vérifie que cela soit le bouton "Play"

            int controlID = LOWORD(wParam);
            int notificationCode = HIWORD(wParam);

            if( controlID == BUTTON_PLAY_ID && notificationCode == BN_CLICKED){
                // Bouton Jouer appuyé
                HWND hButton = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                if( hButton ){
                    DestroyWindow(hButton);
                    if( !Game::getGridSet() ){
                        InvalidateRect(hwnd, nullptr, TRUE);
                        Game::setPaintFlag(INIT_GRID_RECT);
                    }else{
                        SetTimer(hwnd, MOVE_TIMER_ID, SPEED_RECTANGLE, nullptr);
                        timer_set = true;
                        InvalidateRect(hwnd, nullptr, TRUE);
                        REPAINT_NEEDED = CONTINUE_GAME;
                    }
                }
            }

            break;
        }
        case WM_DRAWITEM:{
            // On récupère l'objet qui se dessine

            DRAWITEMSTRUCT* pdis = (DRAWITEMSTRUCT*)lParam;

            if( pdis->CtlID == BUTTON_PLAY_ID){
                playButtonStyle(pdis);
                return TRUE;
            }
            break;
        }
        case WM_CREATE:{
            // Création de l'application
            RECT rect;
            GetClientRect(hwnd, &rect);

            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;

            createPlayButton(hwnd, width, height, BUTTON_PLAY_ID);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            switch( REPAINT_NEEDED ){
                case INIT_APP:{
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    // Remplir toute la zone client avec une couleur de fond
                    RECT rectWindow;
                    GetClientRect(hwnd, &rectWindow);
                    HBRUSH hbrush = CreateSolidBrush(RGB(50,50,50));
                    FillRect(hdc, &rectWindow, hbrush);
                    DeleteObject(hbrush);

                    EndPaint(hwnd, &ps);
                    break;
                }
                case INIT_GRID_RECT:{
                    // Initialisation de toute la grille
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    // Remplir toute la zone client avec une couleur de fond
                    RECT rect;
                    GetClientRect(hwnd, &rect);
                    HBRUSH hbrush = CreateSolidBrush(RGB(50,50,50));
                    FillRect(hdc, &rect, hbrush);
                    DeleteObject(hbrush);
                    DrawGrid(hdc, rect ,RGB(255,255,255));
                    initRect(hdc);  

                    EndPaint(hwnd, &ps);
                    break;
                }
                case RECT_MOVING:{
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    HBRUSH hbrush1 = CreateSolidBrush(RGB(50,50,50));
                    FillRect(hdc, &snake_tail.at(length_snake-1), hbrush1);
                    DeleteObject(hbrush1);

                    HBRUSH hbrush2 = CreateSolidBrush(RGB(255,255,255));
                    FillRect(hdc, &rect, hbrush2);
                    DeleteObject(hbrush2);

                    EndPaint(hwnd, &ps);
                    break;
                }
                case WINDOW_RESIZED:{
                    // On est dans le cas où on redimensionne la fenêtre du jeu
                    // Actualisation du bouton Jouer (positionnement)
                    HWND hButton = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                    if( hButton ){
                        DestroyWindow(hButton);
                    }

                    RECT rectWindow;
                    GetClientRect(hwnd, &rectWindow);

                    int width = rectWindow.right - rectWindow.left;
                    int height = rectWindow.bottom - rectWindow.top;
                    // Création du bouton bien placé
                    createPlayButton(hwnd, width, height);

                    // Il faut recentrer le bouton
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    // Remplissage du fond
                    HBRUSH hbrush = CreateSolidBrush(RGB(50,50,50));
                    FillRect(hdc, &rectWindow, hbrush);
                    DeleteObject(hbrush);

                    // Actualisation de la grille
                    if( grid_set ){
                        UpdateGrid(hdc, rectWindow ,RGB(255,255,255));
                        updateSnake();

                        HBRUSH hbrush = CreateSolidBrush(RGB(255,255,255));
                        FillRect(hdc, &rect, hbrush);

                        for(int i =0; i<length_snake-1; i++){
                            RECT rect_temp;
                            rect_temp = snake_tail.at(i);
                            FillRect(hdc, &rect_temp, hbrush);
                        }
                        DeleteObject(hbrush);
                    }

                    EndPaint(hwnd, &ps);
                    break;
                }
                case CONTINUE_GAME:{
                    RECT rectWindow;
                    GetClientRect(hwnd, &rectWindow);
                    // Il faut recentrer le bouton
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    // Remplissage du fond
                    HBRUSH hbrush1 = CreateSolidBrush(RGB(50,50,50));
                    FillRect(hdc, &rectWindow, hbrush1);
                    DeleteObject(hbrush1);

                    // Actualisation de la grille
                    UpdateGrid(hdc, rectWindow ,RGB(255,255,255));
                    updateSnake();

                    HBRUSH hbrush2 = CreateSolidBrush(RGB(255,255,255));
                    FillRect(hdc, &rect, hbrush2);

                    for(int i =0; i<length_snake-1; i++){
                        RECT rect_temp;
                        rect_temp = snake_tail.at(i);
                        FillRect(hdc, &rect_temp, hbrush2);
                    }
                    DeleteObject(hbrush2);

                    EndPaint(hwnd, &ps);
                    break;
                }
            }
            return 0;
        }
        case WM_SIZE:{
            if( Game::getPlay() ){
                KillTimer(hwnd, MOVE_TIMER_ID);
                Game::setPlay(false);
            }

            // Marquer toute la fenêtre comme invalide pour déclencher un WM_PAINT
            InvalidateRect(hwnd, nullptr, TRUE);
            if( Game::getInitialized() ){
                Game::setPaintFlag(WINDOW_RESIZED);
            }
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"WINDOW";
    Snake snake;
    Grid grid;
    
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Snake",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);
    Game::setInitialized(true);


    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
