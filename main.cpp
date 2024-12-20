#include <windows.h>
#include "Snake.h"
#include "Grid.h"
#include "Game.h"
#include "hmi_functions.h"

#define BUTTON_PLAY_ID 1
#define MOVE_TIMER_ID 2
#define BUTTON_REPLAY_ID 8
#define BUTTON_CONTINUE_ID 9

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
                // On déplace le serpent
                int cellValue = windowData->snake->move(*windowData->grid);
                
                switch( cellValue ){
                    case 0:{
                        // Case vide
                        windowData->snake->popDirection();
                        RECT invalidationRect = windowData->snake->invalidateSnake();
                        Game::setPaintFlag(RECT_MOVING);
                        InvalidateRect(hwnd, &invalidationRect, TRUE);
                        break;
                    }
                    case 1:{
                        // Bombe
                        KillTimer(hwnd, MOVE_TIMER_ID);
                        RECT rectWindow;
                        GetClientRect(hwnd, &rectWindow);
                        int width = rectWindow.right - rectWindow.left;
                        int height = rectWindow.bottom - rectWindow.top;
                        // Création du bouton bien placé
                        playButton(hwnd, L"Rejouer", width, height, BUTTON_REPLAY_ID);
                        Game::setMoving(false);
                        break;
                    }
                    case 2:{
                        // Clous
                        if( windowData->snake->getSize() == 1 ){
                            KillTimer(hwnd, MOVE_TIMER_ID);
                            RECT rectWindow;
                            GetClientRect(hwnd, &rectWindow);
                            int width = rectWindow.right - rectWindow.left;
                            int height = rectWindow.bottom - rectWindow.top;
                            // Création du bouton bien placé
                            playButton(hwnd, L"Rejouer", width, height, BUTTON_REPLAY_ID);
                            Game::setMoving(false);
                        }else{
                            windowData->snake->popDirection();
                            windowData->snake->shrink();
                            RECT invalidationRect = windowData->snake->invalidateSnake();
                            Game::setPaintFlag(RECT_MOVING);
                            InvalidateRect(hwnd, &invalidationRect, TRUE);
                        }
                        break;
                    }
                    case 3:{
                        // Poussière
                        windowData->snake->popDirection();
                        windowData->snake->grow();
                        RECT invalidationRect = windowData->snake->invalidateSnake();
                        Game::setPaintFlag(RECT_MOVING);
                        InvalidateRect(hwnd, &invalidationRect, TRUE);
                        break;
                    }
                    case 4:{
                        // Le serpent se mord la queue
                        KillTimer(hwnd, MOVE_TIMER_ID);
                        RECT rectWindow;
                        GetClientRect(hwnd, &rectWindow);
                        int width = rectWindow.right - rectWindow.left;
                        int height = rectWindow.bottom - rectWindow.top;
                        // Création du bouton bien placé
                        playButton(hwnd, L"Rejouer", width, height, BUTTON_REPLAY_ID);
                        Game::setMoving(false);
                        break;
                    }
                }
            }
        }
        case WM_KEYDOWN:{
            windowData = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            int oldDirection {0};
            windowData->snake->peekDirection(oldDirection);
            if( wParam == VK_RIGHT || wParam == VK_LEFT || wParam == VK_UP || wParam == VK_DOWN ){
                if( !Game::getMoving() ){
                    SetTimer(hwnd, MOVE_TIMER_ID, windowData->snake->getSpeed(), nullptr);
                    Game::setMoving(true);
                }
            }
            switch (wParam){
                case VK_RETURN:{
                    // HWND hButton = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                    // if( hButton ){
                    //     DestroyWindow(hButton);
                    // }
                    // if( Game::getGridSet() ){
                    //     SetTimer(hwnd, MOVE_TIMER_ID, windowData->snake->getSpeed(), nullptr);
                    //     InvalidateRect(hwnd, nullptr, TRUE);
                    //     Game::setPaintFlag(CONTINUE_GAME);
                    // }else{
                    //     InvalidateRect(hwnd, nullptr, TRUE);
                    //     Game::setPaintFlag(INIT_GRID_RECT);
                    // }
                    // break;
                }
                case VK_RIGHT:{
                    if( oldDirection != 4 ){
                        // Se déplace autre que à gauche
                        windowData->snake->addDirection(2);
                    }
                    break;
                }
                case VK_LEFT:{
                    if( oldDirection != 2 ){
                        windowData->snake->addDirection(4);
                    }
                    break;
                }
                case VK_UP:{
                    if( oldDirection != 3 ){
                        windowData->snake->addDirection(1);
                    }
                    break;
                }
                case VK_DOWN:{
                    if( oldDirection != 1 ){
                        windowData->snake->addDirection(3);
                    }
                    break;
                }
                case VK_ESCAPE:{
                    // if( Game::getPlay() ){
                    //     KillTimer(hwnd, MOVE_TIMER_ID);
                    //     RECT rectWindow;
                    //     GetClientRect(hwnd, &rectWindow);

                    //     int width = rectWindow.right - rectWindow.left;
                    //     int height = rectWindow.bottom - rectWindow.top;
                    //     // Création du bouton bien placé
                    //     playButton(hwnd,L"Continuer", width, height, BUTTON_CONTINUE_ID);
                    //     Game::setPlay(false);
                    // }
                    // break;
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
                    InvalidateRect(hwnd, nullptr, TRUE);
                    Game::setPaintFlag(INIT_GRID_RECT);
                }
            }
            if( controlID == BUTTON_CONTINUE_ID && notificationCode == BN_CLICKED){
                // Bouton continuer
            }
            if( controlID == BUTTON_REPLAY_ID && notificationCode == BN_CLICKED){
                // Bouton rejouer
                HWND hButton = GetDlgItem(hwnd, BUTTON_REPLAY_ID);
                if( hButton ){
                    DestroyWindow(hButton);
                    // Réinitialisation du serpent
                    RECT rect;
                    GetClientRect(hwnd, &rect);
                    int width = rect.right - rect.left;
                    int height = rect.bottom - rect.top;
                    windowData->grid->init(width, height);
                    windowData->snake->init(*windowData->grid);
                    std::vector<int> indexImmune = windowData->snake->immunitySnake(*windowData->grid, 5);
                    windowData->grid->fillGridWithElements(indexImmune);
                    InvalidateRect(hwnd, nullptr, TRUE);
                    Game::setPaintFlag(INIT_GRID_RECT);
                }
            }
            break;
        }
        case WM_DRAWITEM:{
            // On récupère l'objet qui se dessine

            DRAWITEMSTRUCT* pdis = (DRAWITEMSTRUCT*)lParam;

            if( pdis->CtlID == BUTTON_PLAY_ID){
                buttonStyle(pdis, L"Jouer");
                return TRUE;
            }
            if( pdis->CtlID == BUTTON_CONTINUE_ID){
                buttonStyle(pdis, L"Continuer");
                return TRUE;
            }
            if( pdis->CtlID == BUTTON_REPLAY_ID){
                buttonStyle(pdis, L"Rejouer");
                return TRUE;
            }
            break;
        }
        case WM_CREATE:{
            // Création de la fenêtre, on va décomposer la structure pour associer le serpent et la grille à la fenêtre
            CREATESTRUCT* pCreateStruct = (CREATESTRUCT*)lParam;
            windowData = (WindowData*)pCreateStruct->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)windowData);

            // Création du bouton play
            RECT rect;
            GetClientRect(hwnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            playButton(hwnd,L"Jouer", width, height, BUTTON_PLAY_ID);

            // Initialisation du snake et de la grille
            // On donne à l'objet Grid les éléments calculés aléatoirement
            windowData->grid->init(width, height);
            windowData->snake->init(*windowData->grid);
            std::vector<int> indexImmune = windowData->snake->immunitySnake(*windowData->grid, 5);
            windowData->grid->fillGridWithElements(indexImmune);

            // On met le paint flag en init app pour n'afficher que la couleur du fond d'écran
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
                    // Affichage initial de l'application
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    // Remplir toute la zone client avec la couleur de fond
                    RECT rectWindow;
                    GetClientRect(hwnd, &rectWindow);
                    Game::clearFigure(rectWindow, hdc, Game::getBackgroundColor());

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
                    Game::clearFigure(rectWindow, hdc, Game::getBackgroundColor());
                    Game::drawGrid(*windowData->grid, hdc, windowData->grid->getGridColor());
                    Game::drawSnake(*windowData->snake, hdc, windowData->snake->getSnakeColor());
                    Game::drawElements(*windowData->grid, hdc, RGB(255,0,0), RGB(255,255,0), RGB(0,255,0));

                    // Set la grille
                    Game::setGridSet(true);

                    EndPaint(hwnd, &ps);
                    break;
                }
                case RECT_MOVING:{
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    // Actualisation de l'affichage
                    Game::updateSnake(*windowData->snake, hdc, windowData->snake->getSnakeColor(), Game::getBackgroundColor());

                    EndPaint(hwnd, &ps);
                    break;
                }
                case WINDOW_RESIZED:{
                    // // On est dans le cas où on redimensionne la fenêtre du jeu
                    // // Actualisation du bouton Jouer (positionnement)

                    // if( Game::getGridSet() ){
                    //     // La grille est mise dans le jeu
                    //     HWND hButton = GetDlgItem(hwnd, BUTTON_PLAY_ID);

                    // }else{
                    //     // Pas de grille dans le jeu 
                    //     HWND hButton = GetDlgItem(hwnd, BUTTON_PLAY_ID);

                    // }


                    // HWND hButton = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                    // if( hButton ){
                    //     DestroyWindow(hButton);
                    // }

                    // RECT rectWindow;
                    // GetClientRect(hwnd, &rectWindow);

                    // int width = rectWindow.right - rectWindow.left;
                    // int height = rectWindow.bottom - rectWindow.top;
                    // // Création du bouton bien placé
                    // playButton(hwnd, width, height, BUTTON_PLAY_ID);

                    // // Il faut recentrer le bouton
                    // PAINTSTRUCT ps;
                    // HDC hdc = BeginPaint(hwnd, &ps);

                    // // On actualise l'affichage
                    // Game::clearFigure(rectWindow, hdc, Game::getBackgroundColor());
                    // Game::drawGrid(*windowData->grid, hdc,windowData->grid->getGridColor());
                    // Game::drawSnake(*windowData->snake, hdc,windowData->snake->getSnakeColor());

                    // EndPaint(hwnd, &ps);
                    break;
                }
                case CONTINUE_GAME:{
                    // On reprend le jeu
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    RECT rectWindow;
                    GetClientRect(hwnd, &rectWindow);

                    // Dessin de la grille
                    Game::clearFigure(rectWindow, hdc, Game::getBackgroundColor());
                    Game::drawGrid(*windowData->grid, hdc, windowData->grid->getGridColor());
                    Game::drawSnake(*windowData->snake, hdc, windowData->snake->getSnakeColor());

                    EndPaint(hwnd, &ps);
                    break;
                }
            }
            return 0;
        }
        case WM_SIZE:{
            // Modification de la taille de la fenêtre
            windowData = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            if( Game::getInitialized() ){
                // Le jeu est initialisé

                if( Game::getMoving() ){
                    // Le jeu est lancé
                    KillTimer(hwnd, MOVE_TIMER_ID);
                    Game::setMoving(false);
                }

                // Récupération des dimensions de la fenêtre
                RECT rectWindow;
                GetClientRect(hwnd, &rectWindow);

                int width = rectWindow.right - rectWindow.left;
                int height = rectWindow.bottom - rectWindow.top;
                windowData->grid->windowChanged(width, height);
                windowData->snake->gridChanged(*windowData->grid);
                Game::setPaintFlag(WINDOW_RESIZED);
                InvalidateRect(hwnd, nullptr, TRUE);
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
