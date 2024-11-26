#include <windows.h>
#include <iostream>
#include <vector>

#define BUTTON_PLAY_ID 1
#define MOVE_TIMER_ID 2
#define SPEED_RECTANGLE 100
#define RATIO_SNAKE 60

#define INIT_GRID_RECT 1
#define INIT_APP 0
#define RECT_MOVING 2
#define WINDOW_RESIZED 3
#define CONTINUE_GAME 4

int number_lines;
int number_columns;
std::vector<int> rect_column;
std::vector<int> rect_line;

int cell_width;
int cell_height;
int offsetX;
int offsetY;

boolean is_moving_right = false;
boolean is_moving_up = false;
boolean is_moving_left = false;
boolean is_moving_down = false;
boolean timer_set = false;
boolean grid_set = false;
boolean initialized = false;

int length_snake = 5;

std::vector<RECT> snake_tail;

RECT rect;

// Variables FLAGS
int REPAINT_NEEDED = INIT_APP;


void createPlayButton(HWND hwnd, int widthWindow, int heightWindow);
void playButtonStyle(DRAWITEMSTRUCT *const pdis);
void updatePlayButtonPosition(HWND hwnd);
void DrawGrid(HDC hdc, RECT rect, COLORREF gridColor);
void initRect(HDC hdc);
void UpdateGrid(HDC hdc, RECT rect, COLORREF gridColor);
void updateSnake();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_TIMER:{
            if (wParam == MOVE_TIMER_ID) {
                RECT rectWindow;
                GetClientRect(hwnd, &rectWindow);

                int rectColumnOld = rect_column.at(0);
                int rectLineOld = rect_line.at(0);

                if( is_moving_right ) {
                    REPAINT_NEEDED = RECT_MOVING;
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
                case VK_RIGHT:
                    if( !is_moving_left ){
                        // Se déplace autre que à gauche
                        is_moving_right =true;
                        is_moving_left = false;
                        is_moving_up = false;
                        is_moving_down = false;
                    }
                    break;
                case VK_LEFT:
                    if( !is_moving_right ){
                        is_moving_left =true;
                        is_moving_right = false;
                        is_moving_up = false;
                        is_moving_down = false;
                    }
                    break;
                case VK_UP:
                    if( !is_moving_down ){
                        is_moving_up =true;
                        is_moving_left = false;
                        is_moving_right = false;
                        is_moving_down = false;
                    }
                    break;
                case VK_DOWN:
                    if( !is_moving_up ){
                        is_moving_down =true;
                        is_moving_left = false;
                        is_moving_right = false;
                        is_moving_up = false;
                    }
                    break;
                case VK_ESCAPE:
                    is_moving_right = false;
                    is_moving_left = false;
                    is_moving_up = false;
                    is_moving_down = false;
                    if( timer_set ){
                        KillTimer(hwnd, MOVE_TIMER_ID);
                        timer_set = false;
                    }
                    break;
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
                    if( !grid_set ){
                        InvalidateRect(hwnd, nullptr, TRUE);
                        REPAINT_NEEDED = INIT_GRID_RECT;
                    }else{
                        
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

            createPlayButton(hwnd, width, height);
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

                        for(int i =0; i<length_snake; i++){
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
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);
                    EndPaint(hwnd, &ps);
                    break;
                }
            }
            return 0;
        }
        case WM_SIZE:{
            is_moving_right = false;
            is_moving_left = false;
            is_moving_up = false;
            is_moving_down = false;
            if( timer_set ){
                KillTimer(hwnd, MOVE_TIMER_ID);
                timer_set = false;
            }

            // Marquer toute la fenêtre comme invalide pour déclencher un WM_PAINT
            InvalidateRect(hwnd, nullptr, TRUE);
            if( initialized ){
                REPAINT_NEEDED = WINDOW_RESIZED;
            }
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"WINDOW";

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
    initialized = true;

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void updateSnake(){
    // Fonction qui met à jour les dimensions des rectangles du serpent

    // On met à jour la tête du serpent
    rect = {offsetX + (rect_column.at(0)-1)*cell_width +2, offsetY + (rect_line.at(0)-1)*cell_height+2, offsetX + rect_column.at(0)*cell_width -1, offsetY + rect_line.at(0)*cell_height - 1};

    // Il faut mettre à jour le corps du serpent
    for(int i = 0; i <length_snake; i++){
        RECT rect_temp;
        rect_temp = {offsetX + (rect_column.at(i+1)-1)*cell_width +2, offsetY + (rect_line.at(i+1)-1)*cell_height+2, offsetX + rect_column.at(i+1)*cell_width -1, offsetY + rect_line.at(i+1)*cell_height - 1};
        snake_tail.at(i) = rect_temp;
    }
}
void UpdateGrid(HDC hdc, RECT rect, COLORREF gridColor){
    // Fonction qui permet de mettre à jour la grille.
    // Définir la brosse pour dessiner les lignes
    HPEN hPen = CreatePen(PS_SOLID, 1, gridColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Largeur et hauteur de la zone cliente
    int clientWidth = rect.right - rect.left;
    int clientHeight = rect.bottom - rect.top;

    // On calcule le cell width à partir du nombre de colonnes et de lignes
    cell_width = clientWidth/number_columns;
    cell_height = clientHeight/number_lines;

    // Calculer le décalage pour centrer la grille
    offsetX = (clientWidth % cell_width) / 2;
    offsetY = (clientHeight % cell_height) / 2;

    // Dessiner les lignes verticales
    for (int x = rect.left + offsetX; x <= rect.right; x += cell_width) {
        MoveToEx(hdc, x, rect.top, nullptr);
        LineTo(hdc, x, rect.bottom);
    }

    // Dessiner les lignes horizontales
    for (int y = rect.top + offsetY; y <= rect.bottom; y += cell_height) {
        MoveToEx(hdc, rect.left, y, nullptr);
        LineTo(hdc, rect.right, y);
    }

    // Restaurer la brosse précédente et nettoyer
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}
void initRect(HDC hdc){ 
    int n_lines = number_lines/2;
    int n_columns = number_columns/2;

    rect_column.push_back(n_columns);
    rect_line.push_back(n_lines);

    rect = {offsetX + (n_columns-1)*cell_width +2, offsetY + (n_lines-1)*cell_height+2, offsetX + n_columns*cell_width -1, offsetY + n_lines*cell_height - 1};
    for(int i = 0; i< length_snake; i++){
        snake_tail.push_back(rect);
        rect_column.push_back(n_columns);
        rect_line.push_back(n_lines);
    }

    HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
}
void DrawGrid(HDC hdc, RECT rect, COLORREF gridColor) {
    // Définir la brosse pour dessiner les lignes
    HPEN hPen = CreatePen(PS_SOLID, 1, gridColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Largeur et hauteur de la zone cliente
    int clientWidth = rect.right - rect.left;
    int clientHeight = rect.bottom - rect.top;

    cell_width = std::max(clientWidth, clientHeight)/RATIO_SNAKE;
    cell_height = cell_width;
    number_lines = clientHeight/cell_width;
    number_columns = clientWidth/cell_width;

    // Calculer le décalage pour centrer la grille
    offsetX = (clientWidth % cell_width) / 2;
    offsetY = (clientHeight % cell_width) / 2;

    // Dessiner les lignes verticales
    for (int x = rect.left + offsetX; x <= rect.right; x += cell_width) {
        MoveToEx(hdc, x, rect.top, nullptr);
        LineTo(hdc, x, rect.bottom);
    }

    // Dessiner les lignes horizontales
    for (int y = rect.top + offsetY; y <= rect.bottom; y += cell_width) {
        MoveToEx(hdc, rect.left, y, nullptr);
        LineTo(hdc, rect.right, y);
    }

    // Restaurer la brosse précédente et nettoyer
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    // On met la grille en true
    grid_set = true;
}
void updatePlayButtonPosition(HWND hwnd){
    RECT rect;
    GetClientRect(hwnd, &rect);

    int widthWindow = rect.right - rect.left;
    int heightWindow = rect.bottom - rect.top;

    int widthButton = widthWindow/14.24;
    int heightButton = heightWindow/14.12;

    int xStartingPoint = (widthWindow - widthButton)/2;
    int yStartingPoint = (heightWindow - heightButton)/2;

    HWND hButton = GetDlgItem(hwnd, BUTTON_PLAY_ID);
    if( hButton ){
        SetWindowPos(hButton, nullptr, xStartingPoint, yStartingPoint, widthButton, heightButton, SWP_NOZORDER);
    }   
}
void createPlayButton(HWND hwnd, int widthWindow, int heightWindow){
    int widthButton = widthWindow/14.24;
    int heightButton = heightWindow/14.12;

    int xStartingPoint = (widthWindow - widthButton)/2;
    int yStartingPoint = (heightWindow - heightButton)/2;

    CreateWindow(
        L"BUTTON", L"Jouer",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        xStartingPoint, yStartingPoint, widthButton, heightButton,
        hwnd, (HMENU)BUTTON_PLAY_ID,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        nullptr);
}
void playButtonStyle(DRAWITEMSTRUCT *const pdis){
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

    DrawText(hdc, L"Jouer", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Restaurer la police d'origine
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}