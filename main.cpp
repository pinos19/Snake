#include <windows.h>
#include "Snake.h"
#include "Grid.h"
#include "Game.h"
#include "hmi_functions.h"

#define BUTTON_PLAY_ID 1
#define MOVE_TIMER_ID 2

#define WINDOW_RESIZED 3
#define INIT_GRID_RECT 4
#define CONTINUE_GAME 5 
#define RECT_MOVING 6
#define INIT_APP 7

struct WindowData{
    Snake* snake;
    Grid* grid;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    WindowData* windowData = nullptr;
    switch (uMsg) {
        case WM_TIMER:{
            windowData = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (wParam == MOVE_TIMER_ID) {
                windowData->snake->move(*windowData->grid);
                Game::setPaintFlag(RECT_MOVING);
                InvalidateRect(hwnd, nullptr, TRUE);
            }
        }
        case WM_KEYDOWN:{
            windowData = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if( wParam == VK_RIGHT || wParam == VK_LEFT || wParam == VK_UP || wParam == VK_DOWN ){
                if( !Game::getPlay() ){
                    SetTimer(hwnd, MOVE_TIMER_ID, windowData->snake->getSpeed(), nullptr);
                    Game::setPlay(true);
                }
            }
            switch (wParam){
                case VK_RETURN:{
                    HWND hButton = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                    if( hButton ){
                        DestroyWindow(hButton);
                    }
                    if( Game::getGridSet() ){
                        SetTimer(hwnd, MOVE_TIMER_ID, windowData->snake->getSpeed(), nullptr);
                        InvalidateRect(hwnd, nullptr, TRUE);
                        Game::setPaintFlag(CONTINUE_GAME);
                    }else{
                        InvalidateRect(hwnd, nullptr, TRUE);
                        Game::setPaintFlag(INIT_GRID_RECT);
                    }
                }
                case VK_RIGHT:{
                    if( windowData->snake->getCurrentDirection() != 4 ){
                        // Se déplace autre que à gauche
                        windowData->snake->setPreviousDirection(windowData->snake->getCurrentDirection());
                        windowData->snake->setCurrentDirection(2);
                    }
                    break;
                }
                case VK_LEFT:{
                    if( windowData->snake->getCurrentDirection() != 2 ){
                        windowData->snake->setPreviousDirection(windowData->snake->getCurrentDirection());
                        windowData->snake->setCurrentDirection(4);
                    }
                    break;
                }
                case VK_UP:{
                    if( windowData->snake->getCurrentDirection() != 3 ){
                        windowData->snake->setPreviousDirection(windowData->snake->getCurrentDirection());
                        windowData->snake->setCurrentDirection(1);
                    }
                    break;
                }
                case VK_DOWN:{
                    if( windowData->snake->getCurrentDirection() != 1 ){
                        windowData->snake->setPreviousDirection(windowData->snake->getCurrentDirection());
                        windowData->snake->setCurrentDirection(3);
                    }
                    break;
                }
                case VK_ESCAPE:{
                    if( Game::getPlay() ){
                        KillTimer(hwnd, MOVE_TIMER_ID);
                    }

                    // Marquer toute la fenêtre comme invalide pour déclencher un WM_PAINT
                    InvalidateRect(hwnd, nullptr, TRUE);
                    if( Game::getInitialized() ){
                        Game::setPaintFlag(WINDOW_RESIZED);
                    }
                    break;
                }  
                default:
                    break;
            }
            break;
        }
        case WM_COMMAND:{
            windowData = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            // On vérifie que cela soit le bouton "Play"

            int controlID = LOWORD(wParam);
            int notificationCode = HIWORD(wParam);

            if( controlID == BUTTON_PLAY_ID && notificationCode == BN_CLICKED){
                // Bouton Jouer appuyé
                HWND hButton = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                if( hButton ){
                    DestroyWindow(hButton);
                    if( Game::getGridSet() ){
                        SetTimer(hwnd, MOVE_TIMER_ID, windowData->snake->getSpeed(), nullptr);
                        InvalidateRect(hwnd, nullptr, TRUE);
                        Game::setPaintFlag(CONTINUE_GAME);
                    }else{
                        InvalidateRect(hwnd, nullptr, TRUE);
                        Game::setPaintFlag(INIT_GRID_RECT);
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
            // Création de la fenêtre, on va décomposer la structure pour associer le serpent et la grille à la fenêtre
            CREATESTRUCT* pCreateStruct = (CREATESTRUCT*)lParam;
            windowData = (WindowData*)pCreateStruct->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)windowData);

            // Création de l'application
            RECT rect;
            GetClientRect(hwnd, &rect);

            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;

            createPlayButton(hwnd, width, height, BUTTON_PLAY_ID);

            // Initialisation du snake et de la grille
            windowData->grid->init(width, height);
            windowData->snake->init(*windowData->grid);
            Game::setPaintFlag(INIT_APP);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            windowData = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            switch( Game::getPaintFlag() ){
                case INIT_APP:{
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    // Remplir toute la zone client avec une couleur de fond
                    RECT rectWindow;
                    GetClientRect(hwnd, &rectWindow);

                    Game::clearFigure(rectWindow, hdc, RGB(50,50,50));

                    EndPaint(hwnd, &ps);
                    break;
                }
                case INIT_GRID_RECT:{
                    // Initialisation de toute la grille
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    // Remplir toute la zone client avec une couleur de fond
                    RECT rectWindow;
                    GetClientRect(hwnd, &rectWindow);

                    // Dessin de la grille
                    Game::clearFigure(rectWindow, hdc, RGB(50,50,50));
                    Game::drawGrid(*windowData->grid, hdc, RGB(255,255,255));
                    Game::drawSnake(*windowData->snake, hdc, RGB(255,255,255));

                    EndPaint(hwnd, &ps);
                    break;
                }
                case RECT_MOVING:{
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);



                    // HBRUSH hbrush1 = CreateSolidBrush(RGB(50,50,50));
                    // FillRect(hdc, &snake_tail.at(length_snake-1), hbrush1);
                    // DeleteObject(hbrush1);

                    // HBRUSH hbrush2 = CreateSolidBrush(RGB(255,255,255));
                    // FillRect(hdc, &rect, hbrush2);
                    // DeleteObject(hbrush2);

                    EndPaint(hwnd, &ps);
                    break;
                }
                case WINDOW_RESIZED:{
                    // On est dans le cas où on redimensionne la fenêtre du jeu
                    // Actualisation du bouton Jouer (positionnement)
                    // HWND hButton = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                    // if( hButton ){
                    //     DestroyWindow(hButton);
                    // }

                    // RECT rectWindow;
                    // GetClientRect(hwnd, &rectWindow);

                    // int width = rectWindow.right - rectWindow.left;
                    // int height = rectWindow.bottom - rectWindow.top;
                    // // Création du bouton bien placé
                    // createPlayButton(hwnd, width, height);

                    // // Il faut recentrer le bouton
                    // PAINTSTRUCT ps;
                    // HDC hdc = BeginPaint(hwnd, &ps);

                    // // Remplissage du fond
                    // HBRUSH hbrush = CreateSolidBrush(RGB(50,50,50));
                    // FillRect(hdc, &rectWindow, hbrush);
                    // DeleteObject(hbrush);

                    // // Actualisation de la grille
                    // if( grid_set ){
                    //     UpdateGrid(hdc, rectWindow ,RGB(255,255,255));
                    //     updateSnake();

                    //     HBRUSH hbrush = CreateSolidBrush(RGB(255,255,255));
                    //     FillRect(hdc, &rect, hbrush);

                    //     for(int i =0; i<length_snake-1; i++){
                    //         RECT rect_temp;
                    //         rect_temp = snake_tail.at(i);
                    //         FillRect(hdc, &rect_temp, hbrush);
                    //     }
                    //     DeleteObject(hbrush);
                    // }

                    // EndPaint(hwnd, &ps);
                    break;
                }
                case CONTINUE_GAME:{
                    // RECT rectWindow;
                    // GetClientRect(hwnd, &rectWindow);
                    // // Il faut recentrer le bouton
                    // PAINTSTRUCT ps;
                    // HDC hdc = BeginPaint(hwnd, &ps);

                    // // Remplissage du fond
                    // HBRUSH hbrush1 = CreateSolidBrush(RGB(50,50,50));
                    // FillRect(hdc, &rectWindow, hbrush1);
                    // DeleteObject(hbrush1);

                    // // Actualisation de la grille
                    // UpdateGrid(hdc, rectWindow ,RGB(255,255,255));
                    // updateSnake();

                    // HBRUSH hbrush2 = CreateSolidBrush(RGB(255,255,255));
                    // FillRect(hdc, &rect, hbrush2);

                    // for(int i =0; i<length_snake-1; i++){
                    //     RECT rect_temp;
                    //     rect_temp = snake_tail.at(i);
                    //     FillRect(hdc, &rect_temp, hbrush2);
                    // }
                    // DeleteObject(hbrush2);

                    // EndPaint(hwnd, &ps);
                    break;
                }
            }
            return 0;
        }
        case WM_SIZE:{
            if( Game::getPlay() ){
                // On met en pause
                KillTimer(hwnd, MOVE_TIMER_ID);
                Game::setPlay(false);
            }
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
    Snake* snake = new Snake();
    Grid* grid = new Grid();

    // Structure contenant la grille et le serpent
    WindowData* windowData = new WindowData{snake, grid};
    Game::init();
    
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
        nullptr, nullptr, hInstance, windowData
    );

    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);
    Game::setInitialized(true);


    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Suppression des pointeurs vers le snake et la grid
    delete(windowData->grid);
    delete(windowData->snake);
    delete(windowData);
    return 0;
}
