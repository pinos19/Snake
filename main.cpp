#include <windows.h>
#include "Snake.h"
#include "Grid.h"
#include "Game.h"
#include "hmi_functions.h"

#define BUTTON_PLAY_ID 1
#define MOVE_TIMER_ID 2


std::ostream &operator<<(std::ostream &os, const Grid::TileContent tileContent){
    switch(tileContent){
        case Grid::TileContent::Bomb:{
            os << "Bomb";
            break;
        }
        case Grid::TileContent::Nail:{
            os << "Nail";
            break;
        }
        case Grid::TileContent::Dust:{
            os << "Dust";
            break;
        }
        case Grid::TileContent::Empty:{
            os << "Empty";
            break;
        }
        case Grid::TileContent::Snake:{
            os << "Snake";
            break;
        }
    }
    return os;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Game* game = nullptr;
    switch (uMsg) {
        case WM_TIMER:{
            game = (Game*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            if (wParam == MOVE_TIMER_ID) {
                // We move the snake

                if( game->GameSnake.getCurrentDirection() != Snake::Direction::None ){
                    Grid::TileContent tileContent = game->GameSnake.move(game->GameGrid);
                    // std::cout << tileContent << std::endl;
                    switch( tileContent ){
                        case Grid::TileContent::Bomb:{
                            // Kill timer
                            KillTimer(hwnd, MOVE_TIMER_ID);

                            // Display of teh button to play again
                            RECT rectWindow;
                            GetClientRect(hwnd, &rectWindow);
                            int width = rectWindow.right - rectWindow.left;
                            int height = rectWindow.bottom - rectWindow.top;
                            playButton(hwnd, L"Jouer", width, height, BUTTON_PLAY_ID);

                            // Initialization of game for the game to restart properly
                            game->GameState = Game::StateGame::Dead;
                            game->GamePaintFlag = Game::PaintFlag::InitGrid;
                            game->GameSnake.init(game->GameGrid);
                            game->GameGrid.fillGridWithElements(game->GameSnake.immunitySnake(game->GameGrid, 5));
                            game->Score = 0;
                            break;
                        }
                        case Grid::TileContent::Nail:{
                            if( game->GameSnake.getIsAlive() ){
                                // Snake still alive
                                game->GameSnake.popDirection();
                                RECT invalidationRect = game->GameSnake.invalidateSnake(game->GameGrid);
                                InvalidateRect(hwnd, &invalidationRect, TRUE);
                            }else{
                                // Snake dead
                                KillTimer(hwnd, MOVE_TIMER_ID);

                                // Display of teh button to play again
                                RECT rectWindow;
                                GetClientRect(hwnd, &rectWindow);
                                int width = rectWindow.right - rectWindow.left;
                                int height = rectWindow.bottom - rectWindow.top;
                                playButton(hwnd, L"Jouer", width, height, BUTTON_PLAY_ID);

                                // Initialization of game for the game to restart properly
                                game->GameState = Game::StateGame::Dead;
                                game->GamePaintFlag = Game::PaintFlag::InitGrid;
                                game->GameSnake.init(game->GameGrid);
                                game->GameGrid.fillGridWithElements(game->GameSnake.immunitySnake(game->GameGrid, 5));
                                game->Score = 0;
                            }
                            break;
                        }
                        case Grid::TileContent::Dust:{
                            game->GameSnake.popDirection();
                            RECT invalidationRect = game->GameSnake.invalidateSnake(game->GameGrid);
                            InvalidateRect(hwnd, &invalidationRect, TRUE);
                            break;
                        }
                        case Grid::TileContent::Snake:{
                            // Snake dead
                            // Kill timer
                            KillTimer(hwnd, MOVE_TIMER_ID);

                            // Display of teh button to play again
                            RECT rectWindow;
                            GetClientRect(hwnd, &rectWindow);
                            int width = rectWindow.right - rectWindow.left;
                            int height = rectWindow.bottom - rectWindow.top;
                            playButton(hwnd, L"Jouer", width, height, BUTTON_PLAY_ID);

                            // Initialization of game for the game to restart properly
                            game->GameState = Game::StateGame::Dead;
                            game->GamePaintFlag = Game::PaintFlag::InitGrid;
                            game->GameSnake.init(game->GameGrid);
                            game->GameGrid.fillGridWithElements(game->GameSnake.immunitySnake(game->GameGrid, 5));
                            game->Score = 0;
                            break;
                        }
                        case Grid::TileContent::Empty:{
                            game->GameSnake.popDirection();
                            RECT invalidationRect = game->GameSnake.invalidateSnake(game->GameGrid);
                            InvalidateRect(hwnd, &invalidationRect, TRUE);
                            break;
                        }
                    }
                }
            }
            break;
        }
        case WM_KEYDOWN:{
            game = (Game*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            switch(wParam){
                case VK_RIGHT:{
                    if( game->GameSnake.getCurrentDirection() != Snake::Direction::Left  && game->GameState == Game::StateGame::Playing){
                        game->GameSnake.addDirection(Snake::Direction::Right);
                        game->GamePaintFlag = Game::PaintFlag::Moving;
                    }
                    break;
                }
                case VK_LEFT:{
                    if( game->GameSnake.getCurrentDirection() != Snake::Direction::Right && game->GameState == Game::StateGame::Playing){
                        game->GameSnake.addDirection(Snake::Direction::Left);
                        game->GamePaintFlag = Game::PaintFlag::Moving;
                    }
                    break;
                }
                case VK_UP:{
                    if( game->GameSnake.getCurrentDirection() != Snake::Direction::Down && game->GameState == Game::StateGame::Playing){
                        game->GameSnake.addDirection(Snake::Direction::Up);
                        game->GamePaintFlag = Game::PaintFlag::Moving;
                    }
                    break;
                }
                case VK_DOWN:{
                    if( game->GameSnake.getCurrentDirection() != Snake::Direction::Up && game->GameState == Game::StateGame::Playing){
                        game->GameSnake.addDirection(Snake::Direction::Down);
                        game->GamePaintFlag = Game::PaintFlag::Moving;
                    }
                    break;
                }
                case VK_ESCAPE:{
                    // Pause the game
                    if( game->GameState == Game::StateGame::Playing ){
                        game->GameState = Game::StateGame::Pause;
                        RECT rectWindow;
                        GetClientRect(hwnd, &rectWindow);

                        int width = rectWindow.right - rectWindow.left;
                        int height = rectWindow.bottom - rectWindow.top;

                        // Creation of the CONTINUE button
                        playButton(hwnd, L"Jouer", width, height, BUTTON_PLAY_ID);
                        KillTimer(hwnd, MOVE_TIMER_ID);
                    }
                    break;
                }
                case VK_RETURN:{
                    if( game->GameState == Game::StateGame::Pause || game->GameState == Game::StateGame::Dead || game->GameState == Game::StateGame::Menu ){
                        HWND hButtonPlay = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                        if( hButtonPlay ){
                            // We delete the PLAY button and play
                            // std::cout << "Deletion !" << std::endl;
                            DestroyWindow(hButtonPlay);
                            switch(game->GameState){
                                case Game::StateGame::Pause:{
                                    game->GamePaintFlag = Game::PaintFlag::ResumeGame;
                                    break;
                                }
                                case Game::StateGame::Dead:{
                                    game->GamePaintFlag = Game::PaintFlag::InitGrid;
                                    // We need to reset the snake and the elements
                                    game->GameSnake.init(game->GameGrid);
                                    game->GameGrid.fillGridWithElements(game->GameSnake.immunitySnake(game->GameGrid, 5));
                                    break;
                                }
                                case Game::StateGame::Menu:{
                                    game->GamePaintFlag = Game::PaintFlag::InitGrid;
                                    game->GameGrid.fillGridWithElements(game->GameSnake.immunitySnake(game->GameGrid, 5));
                                    break;
                                }
                            }
                            game->GameState = Game::StateGame::Playing;
                            InvalidateRect(hwnd, nullptr, TRUE);
                            SetTimer(hwnd, MOVE_TIMER_ID, game->GameSnake.getSpeed(), nullptr);
                        }
                    }
                    break;
                }
            }
            break;
        }
        case WM_COMMAND:{
            game = (Game*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            // Check for the input

            int controlID = LOWORD(wParam);
            int notificationCode = HIWORD(wParam);

            if( controlID == BUTTON_PLAY_ID && notificationCode == BN_CLICKED){
                // Play button clicked
                HWND hButtonPlay = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                if( hButtonPlay ){
                    // We delete the PLAY button and play
                    // std::cout << "Deletion !" << std::endl;
                    DestroyWindow(hButtonPlay);
                    switch(game->GameState){
                        case Game::StateGame::Pause:{
                            game->GamePaintFlag = Game::PaintFlag::ResumeGame;
                            break;
                        }
                        case Game::StateGame::Dead:{
                            game->GamePaintFlag = Game::PaintFlag::InitGrid;
                            // We need to reset the snake and the elements
                            game->GameSnake.init(game->GameGrid);
                            game->GameGrid.fillGridWithElements(game->GameSnake.immunitySnake(game->GameGrid, 5));
                            break;
                        }
                        case Game::StateGame::Menu:{
                            game->GamePaintFlag = Game::PaintFlag::InitGrid;
                            game->GameGrid.fillGridWithElements(game->GameSnake.immunitySnake(game->GameGrid, 5));
                            break;
                        }
                    }
                    game->GameState = Game::StateGame::Playing;
                    InvalidateRect(hwnd, nullptr, TRUE);
                    SetTimer(hwnd, MOVE_TIMER_ID, game->GameSnake.getSpeed(), nullptr);
                }
            }
            break;
        }
        case WM_DRAWITEM:{
            // On récupère l'objet qui se dessine

            DRAWITEMSTRUCT* pdis = (DRAWITEMSTRUCT*)lParam;

            if( pdis->CtlID == BUTTON_PLAY_ID){
                buttonStyle(pdis, L"Jouer");
            }
            break;
        }
        case WM_CREATE:{
            // Creation of the window, we are going to associate the game pointer to the window
            CREATESTRUCT* pCreateStruct = (CREATESTRUCT*)lParam;
            game = (Game*)pCreateStruct->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)game);

            // Creation of the button play
            RECT rect;
            GetClientRect(hwnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            playButton(hwnd,L"Jouer", width, height, BUTTON_PLAY_ID);

            // Initialization
            game->init(width, height);
            return 0;
        }
        case WM_DESTROY:
            KillTimer(hwnd, MOVE_TIMER_ID);
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            game = (Game*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            switch( game->GamePaintFlag){
                case Game::PaintFlag::InitApp:{
                    // Initial display of the application
                    // std::cout << "InitApp" << std::endl;
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    // Fill the background color
                    RECT rectWindow;
                    GetClientRect(hwnd, &rectWindow);
                    game->clearFigure(rectWindow, hdc);

                    EndPaint(hwnd, &ps);
                    break;
                }
                case Game::PaintFlag::InitGrid:{
                    // std::cout << "InitGrid" << std::endl;
                    // Initialization of the grid
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    RECT rectWindow;
                    GetClientRect(hwnd, &rectWindow);
                    // Drawing of the grid and the snake
                    game->clearFigure(rectWindow, hdc);
                    game->drawGrid(hdc);
                    game->drawSnake(hdc);
                    game->drawElements(hdc,game->GameGrid.getIndexBombs(), RGB(255,0,0));
                    game->drawElements(hdc,game->GameGrid.getIndexNails(), RGB(255,255,0));
                    game->drawElements(hdc,game->GameGrid.getIndexDusts(), RGB(0,255,0));

                    EndPaint(hwnd, &ps);
                    break;
                }
                case Game::PaintFlag::Moving:{
                    // std::cout << "Moving" << std::endl;
                    // The snake is moving
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    // Actualisation de l'affichage
                    game->updateSnake(hdc);

                    EndPaint(hwnd, &ps);
                    break;
                }
                case Game::PaintFlag::ResumeGame:{
                    // std::cout << "Moving" << std::endl;
                    // The snake is moving

                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    RECT rectWindow;
                    GetClientRect(hwnd, &rectWindow);
                    // Drawing of the grid and the snake
                    game->clearFigure(rectWindow, hdc);
                    game->drawGrid(hdc);
                    game->drawSnake(hdc);
                    game->drawElements(hdc,game->GameGrid.getIndexBombs(), RGB(255,0,0));
                    game->drawElements(hdc,game->GameGrid.getIndexNails(), RGB(255,255,0));
                    game->drawElements(hdc,game->GameGrid.getIndexDusts(), RGB(0,255,0));

                    EndPaint(hwnd, &ps);

                    game->GamePaintFlag = Game::PaintFlag::Moving;
                    break;
                }
                case Game::PaintFlag::WinResized:{
                    // std::cout << "Moving" << std::endl;
                    // The snake is moving
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);

                    RECT rectWindow;
                    GetClientRect(hwnd, &rectWindow);

                    switch( game->GameState ){
                        case Game::StateGame::Playing:{
                            game->clearFigure(rectWindow, hdc);
                            game->drawGrid(hdc);
                            game->drawSnake(hdc);
                            game->drawElements(hdc,game->GameGrid.getIndexBombs(), RGB(255,0,0));
                            game->drawElements(hdc,game->GameGrid.getIndexNails(), RGB(255,255,0));
                            game->drawElements(hdc,game->GameGrid.getIndexDusts(), RGB(0,255,0));
                            break;
                        }
                        case Game::StateGame::Menu:{
                            game->clearFigure(rectWindow, hdc);
                            break;
                        }
                        case Game::StateGame::Dead:{
                            game->clearFigure(rectWindow, hdc);
                            game->drawGrid(hdc);
                            game->drawSnake(hdc);
                            game->drawElements(hdc,game->GameGrid.getIndexBombs(), RGB(255,0,0));
                            game->drawElements(hdc,game->GameGrid.getIndexNails(), RGB(255,255,0));
                            game->drawElements(hdc,game->GameGrid.getIndexDusts(), RGB(0,255,0));
                            break;
                        }
                        case Game::StateGame::Pause:{
                            game->clearFigure(rectWindow, hdc);
                            game->drawGrid(hdc);
                            game->drawSnake(hdc);
                            game->drawElements(hdc,game->GameGrid.getIndexBombs(), RGB(255,0,0));
                            game->drawElements(hdc,game->GameGrid.getIndexNails(), RGB(255,255,0));
                            game->drawElements(hdc,game->GameGrid.getIndexDusts(), RGB(0,255,0));
                            break;
                        }
                    }
                    EndPaint(hwnd, &ps);
                    break;
                }
                
            }
            break;
        }
        case WM_SIZE:{
            // Window size changed
            game = (Game*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            if( game->Initialized ){
                // The window is initialized
                switch( game->GameState ){
                    case Game::StateGame::Playing:{
                        // The snake is playing
                        KillTimer(hwnd, MOVE_TIMER_ID);

                        // Display of the button to play again
                        RECT rectWindow;
                        GetClientRect(hwnd, &rectWindow);
                        int width = rectWindow.right - rectWindow.left;
                        int height = rectWindow.bottom - rectWindow.top;
                        playButton(hwnd, L"Jouer", width, height, BUTTON_PLAY_ID);

                        // Pause the game and modify the display of the app
                        game->GameState = Game::StateGame::Pause;
                        game->GameGrid.windowChanged(width, height);
                        game->GamePaintFlag = Game::PaintFlag::WinResized;
                        InvalidateRect(hwnd, nullptr, TRUE);
                        break;
                    }
                    case Game::StateGame::Dead:{
                        // Display of teh button to play again
                        RECT rectWindow;
                        GetClientRect(hwnd, &rectWindow);
                        HWND hButtonPlay = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                        if( hButtonPlay ){
                            DestroyWindow(hButtonPlay);
                        }
                        int width = rectWindow.right - rectWindow.left;
                        int height = rectWindow.bottom - rectWindow.top;
                        playButton(hwnd, L"Jouer", width, height, BUTTON_PLAY_ID);

                        // Pause the game and modify the display of the app
                        game->GameGrid.windowChanged(width, height);
                        game->GamePaintFlag = Game::PaintFlag::WinResized;
                        InvalidateRect(hwnd, nullptr, TRUE);
                        break;
                    }
                    case Game::StateGame::Pause:{
                        // Display of teh button to play again
                        RECT rectWindow;
                        GetClientRect(hwnd, &rectWindow);
                        HWND hButtonPlay = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                        if( hButtonPlay ){
                            DestroyWindow(hButtonPlay);
                        }
                        int width = rectWindow.right - rectWindow.left;
                        int height = rectWindow.bottom - rectWindow.top;
                        playButton(hwnd, L"Jouer", width, height, BUTTON_PLAY_ID);

                        // Pause the game and modify the display of the app
                        game->GameGrid.windowChanged(width, height);
                        game->GamePaintFlag = Game::PaintFlag::WinResized;
                        InvalidateRect(hwnd, nullptr, TRUE);
                        break;
                    }
                    case Game::StateGame::Menu:{
                        RECT rectWindow;
                        GetClientRect(hwnd, &rectWindow);
                        HWND hButtonPlay = GetDlgItem(hwnd, BUTTON_PLAY_ID);
                        if( hButtonPlay ){
                            DestroyWindow(hButtonPlay);
                        }
                        int width = rectWindow.right - rectWindow.left;
                        int height = rectWindow.bottom - rectWindow.top;
                        playButton(hwnd, L"Jouer", width, height, BUTTON_PLAY_ID);

                        game->GameGrid.init(width, height);
                        game->GamePaintFlag = Game::PaintFlag::WinResized;
                        InvalidateRect(hwnd, nullptr, TRUE);
                        break;
                    }
                }
            }
            break;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"WINDOW";

    Game* game = new Game();
    
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
        nullptr, nullptr, hInstance, game
    );

    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);
    game->Initialized = true;


    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Deletion of the game pointer
    delete(game);
    return 0;
}
