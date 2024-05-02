// Headers
#include <iostream>
#include <cmath>
#include <string>
#include <Windows.h>
#include <conio.h>
#define MATH_PI 3.14159265358979323846


// Console Clearage Function
void clearConsole(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD restartCursorPosition = {0, 0};
    SetConsoleCursorPosition(hConsole, restartCursorPosition);
}

void cursorPosition(short x, short y){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD restartCursorPosition = {x, y};
    SetConsoleCursorPosition(hConsole, restartCursorPosition);
}

// Gtid & Screen
double FOV = 100;
const int ScreenHEIGHT = 40;
const int ScreenWIDTH = 150;
const int GridHEIGHT = 15;
const int GridWIDTH = 11;
// Player
double playerPosH = 8 - 1;
double playerPosW = 6 - 1;
double playerDir = 0;
double playerSpeed = 0.5;
char keyPress = ' ';
// Additional Variables
int wallHeight = 1;
int distanceToProjectionPlane = ScreenHEIGHT / 2;
double fixedRayDistanceToWall;
double rayDistanceToWall;

// Arrays
char Screen[ScreenHEIGHT][ScreenWIDTH];
int Map[GridHEIGHT][GridWIDTH] =  {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
// Map = Array of Walls to Change Grid back to official values after player walks over the walls
int Grid[GridHEIGHT][GridWIDTH];

// ===================================== G R I D =====================================

void checkValidplayerDir(){
    if(playerDir > 360){
        playerDir -= 360;
    }
    if(playerDir < 0){
        playerDir += 360;
    }
}

void gridInitialize(){
    for(int h = 0; h < GridHEIGHT; h++){
        for(int w = 0; w < GridWIDTH; w++){
            Grid[h][w] = Map[h][w];
        }
    }
}

void updateGrid(){
    // Update the Grid Player Position
    for(int h = 0; h < GridHEIGHT; h++){
        for(int w = 0; w < GridWIDTH; w++){
            if(h == round(playerPosH) && w == round(playerPosW)){
                Grid[h][w] = 2;
            }
            else{
                if(Grid[h][w] == 2){
                    Grid[h][w] = Map[h][w];
                }
            }
        }
    }
}

void displayGrid(){
    // Displays the Grid on Screen
    system("cls");
    for(int h = 0; h < GridHEIGHT; h++){
        for(int w = 0; w < GridWIDTH; w++){
            std::cout << Grid[h][w];
        }
        std::cout << "\n";
    }
    std::cout << fmod(playerDir, 360) << '\n';
}

// ======================= S C R E E N   S I Z E   S E T U P =========================

void screenSizeSetup(){
    while(!_kbhit()){
        system("cls");
        for(int h = 0; h < ScreenHEIGHT + 7; h++){
            for(int w = 0; w < ScreenWIDTH; w++){
                if(h == 0 || h == ScreenHEIGHT + 7 - 1){
                    std::cout << "=";
                } else if(w == 0 || w == ScreenWIDTH - 1){
                    std::cout << "|";
                } else {
                    std::cout << " ";
                }
            }
            std::cout << '\n';
        }
        cursorPosition(ScreenWIDTH / 20, ((ScreenHEIGHT + 7) / 10) - 1);
        std::cout << "\r|   If you cannot see the entire rectangle,\n";
        std::cout << "\r|   set good font size in command line settings.\n\n";
        std::cout << "\r|   More info on my github:\n";
        std::cout << "\r|   github.com/Nixhekoo/NTR-NixhekoosTerminalRaycaster_CPP\n";
        std::cout << "\r|   (You can copy the link using cursor.)\n\n";
        std::cout << "\r|   Hold any key for more then 2 seconds to continue to the Game";
        cursorPosition(ScreenHEIGHT + 7, ScreenWIDTH);
        Sleep(2000);
    }
}

// =================================== S C R E E N ===================================

void setupScreen(){
    for(int h = 0; h < ScreenHEIGHT; h++){
        for(int w = 0; w < ScreenWIDTH; w++){
            Screen[h][w] = ' ';
        }
    }
}

void displayScreen(){
    clearConsole();
    for(int h = 0; h < ScreenHEIGHT; h++){
        for(int w = 0; w < ScreenWIDTH; w++){
            if(h == 0 || h == ScreenHEIGHT - 1){
                std::cout << "=";
            } else if(w == 0 || w == ScreenWIDTH - 1){
                std::cout << "|";
            } else {
                std::cout << Screen[h][w];
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\n\nKeybinds:\n[WASD] = Move\n[JL] = Rotate\n[X] = Clear Screen (If any bugs appear outside main 'Rectangle')";
}

void singleRay(double rayNum){
    // Ray Variables
    double rayDirection = ((playerDir - (FOV / 2)) + rayNum);
    double rayPosH = playerPosH;
    double rayPosW = playerPosW;
    int intRayPosH;
    int intRayPosW;
    rayDistanceToWall = 0;
    bool rayTouchingWall = false;
    double raySpeed = 0.15;

    // Ray Cast
    while(!rayTouchingWall){
        rayPosH += raySpeed * sin(rayDirection * (MATH_PI / 180));
        rayPosW += raySpeed * cos(rayDirection * (MATH_PI / 180));
        intRayPosH = round(rayPosH);
        intRayPosW = round(rayPosW);
        if(Grid[intRayPosH][intRayPosW] == 1){
            rayTouchingWall = true;
        } else {
            rayDistanceToWall += raySpeed;
        }
    }
    fixedRayDistanceToWall = rayDistanceToWall * cos((rayDirection - playerDir) * (MATH_PI / 180));
}

void raycast(){
    for(double i = 0; i < FOV; i += 0.65){
        singleRay(i);
        int selectedScreenWidth = (ScreenWIDTH / FOV) * i;
        int startingCharHeight = (ScreenHEIGHT / 2) - (wallHeight / fixedRayDistanceToWall) * distanceToProjectionPlane;
        int endingCharHeight = (ScreenHEIGHT / 2) + (wallHeight / fixedRayDistanceToWall) * distanceToProjectionPlane;
        for(int j = 0; j < ScreenHEIGHT; j++){
            if(j >= startingCharHeight && j <= endingCharHeight){
                if(round(fixedRayDistanceToWall) > 7){
                    Screen[j][selectedScreenWidth] = ' ';
                } else if(round(fixedRayDistanceToWall) == 7){
                    Screen[j][selectedScreenWidth] = '.';
                } else if(round(fixedRayDistanceToWall) == 6){
                    Screen[j][selectedScreenWidth] = '-';
                } else if(round(fixedRayDistanceToWall) == 5){
                    Screen[j][selectedScreenWidth] = '=';
                } else if(round(fixedRayDistanceToWall) == 4){
                    Screen[j][selectedScreenWidth] = '+';
                } else if(round(fixedRayDistanceToWall) == 3){
                    Screen[j][selectedScreenWidth] = '*';
                } else if(round(fixedRayDistanceToWall) == 2){
                    Screen[j][selectedScreenWidth] = '#';
                } else if(round(fixedRayDistanceToWall) == 1){
                    Screen[j][selectedScreenWidth] = '%';
                } else if(round(fixedRayDistanceToWall) == 0){
                    Screen[j][selectedScreenWidth] = '@';
                }
            } else {
                Screen[j][selectedScreenWidth] = ' ';
            }
        }
    }
}

// "@%#*+=-:. "

// ============================ P L A Y E R   M O V E M E N T ========================

void playerMovement(){
    // Checks for key presses
    keyPress = ' ';
    for(int i = 0; i <= 200; i+=50){
        Sleep(25);
        if (_kbhit()){
            keyPress = _getch();
        }
    }

    // Moves the player
    if(keyPress != ' '){
        switch(keyPress){
            case 'j':
                playerDir -= 10;
                checkValidplayerDir();
                break;
            case 'x':
                system("cls");
                break;
            case 'l':
                playerDir += 10;
                checkValidplayerDir();
                break;
            case 'w':
                playerPosH += playerSpeed * sin(playerDir * (MATH_PI / 180));
                playerPosW += playerSpeed * cos(playerDir * (MATH_PI / 180));
                break;
            case 's':
                playerPosH -= playerSpeed * sin(playerDir * (MATH_PI / 180));
                playerPosW -= playerSpeed * cos(playerDir * (MATH_PI / 180));
                break;
            case 'a':
                playerPosH += playerSpeed * sin((playerDir - 90) * (MATH_PI / 180));
                playerPosW += playerSpeed * cos((playerDir - 90) * (MATH_PI / 180));
                break;
            case 'd':
                playerPosH += playerSpeed * sin((playerDir + 90) * (MATH_PI / 180));
                playerPosW += playerSpeed * cos((playerDir + 90) * (MATH_PI / 180));
                break;
            default:
                break;
        }
    }
}

// ===================================== M A I N =====================================

int main(){
    screenSizeSetup();
    setupScreen();
    gridInitialize();
    while(1 == 1){
        playerMovement();
        updateGrid();
        raycast();
        displayScreen();
    }
    return 0;
}