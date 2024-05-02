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
const int GridHEIGHT = 20;
const int GridWIDTH = 20;
const int renderDistance = 7;
// Player
double playerPosH = 8.2; //         208.2;   =  Close to the         |    58.2;   =  Close to the         |     28.2;    =   Close to the            |    8.2;  =    Spawn
double playerPosW = 7.0; //        -3992.0;  =  Death Dimension     |    -992.0;  =  Return Dimension     |     -392.0;   =   Undefined Dimension      |    7.0;  =  of the World
double playerDir = 230;
double playerSpeed = 0.5;
char keyPress = ' ';
// Additional Variables
int wallHeight = 1;
int distanceToProjectionPlane = ScreenHEIGHT / 2;
double fixedRayDistanceToWall;
double rayDistanceToWall;
// Special Blocks
int rayTouchingBlockType = 0;
std::string doorText = " DOOR";
// Dimensions
std::string limbodimensionText = " LIMBO";
std::string undefineddimensionText = " you are not supposed to be here. go back before its too late. im warning you.";
std::string returndimensionText = " 01000100 01000101 01000001 01010100 01001000";
std::string deathdimensionText = " NO RETURN";
int deathdimensionTimer = -1;

// Arrays
char Screen[ScreenHEIGHT][ScreenWIDTH];
int Map[GridHEIGHT][GridWIDTH] =  {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
                                   {1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                   {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2},
                                   {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
                                   {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
// Map = Array of Walls to Change Grid back to official values after player walks over the walls
int Grid[GridHEIGHT][GridWIDTH];

void deathDimensionAccessed(){
    clearConsole();
    std::cout << "+----------------------------------------------------------------------------------------------------------------------------------------------------+\n";
    std::cout << "| D E A T H   D E A T H   D E A T H   D E A T H   D E A T H  .-+*#####**+++===-:..  D E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D E A|\n";
    std::cout << "|   D E A T H   D E A T H   D E A T H   D E A T H   D E  .-+%%%@@%#####*++**++=====-..  E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D E|\n";
    std::cout << "| H   D E A T H   D E A T H   D E A T H   D E A T H    .=#@@@@%%%%#####%%##**#*****+=--.  E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D|\n";
    std::cout << "| T H   D E A T H   D E A T H   D E A T H   D E A T  :*%@@@@@@@@@@@@@%@@@@@@@%%%###**=--:.    A T H   D E A T H   D E A T H   D E A T H   D E A T H  |\n";
    std::cout << "| A T H   D E A T H   D E A T H   D E A T H   D E  .*@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%#++=:::.   E A T H   D E A T H   D E A T H   D E A T H   D E A T H|\n";
    std::cout << "| E A T H   D E A T H   D E A T H   D E A T H     .%@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%*++=:::.   D E A T H   D E A T H   D E A T H   D E A T H   D E A T|\n";
    std::cout << "| D E A T H   D E A T H   D E A T H   D E A T H   .#@@@@@@@@@@@@@@@@@@@@@@@@@@%@@%#*+=--:::..   D E A T H   D E A T H   D E A T H   D E A T H   D E A|\n";
    std::cout << "|   D E A T H   D E A T H   D E A T H   D E A T   #@@@@@@@@@@@@@@@@@@@@@@@@@@%##%%**+==---:.  H   D E A T H   D E A T H   D E A T H   D E A T H   D E|\n";
    std::cout << "| H   D E A T H   D E A T H   D E A T H   D E A  :%%%%@@@@@@@@@@@@@@@@@@@@@@@%#%@%%#*+=-=-:.  T H   D E A T H   D E A T H   D E A T H   D E A T H   D|\n";
    std::cout << "| T H   D E A T H   D E A T H   D E A T H   D E  -#%%#@@@@@@@@@@@@@@@@%%@@@@@@@%%***++++=-.   A T H   D E A T H   D E A T H   D E A T H   D E A T H  |\n";
    std::cout << "| A T H   D E A T H   D E A T H   D E A T H   D  -#%@@@@@@@@@@@@@%%%%%#*##%@@%*==--===+++-:.  E A T H   D E A T H   D E A T H   D E A T H   D E A T H|\n";
    std::cout << "| E A T H   D E A T H   D E A T H   D E A T H    :*%##@@@@@@@@#=-:-::::-=++++=.  ..::--====:.   E A T H   D E A T H   D E A T H   D E A T H   D E A T|\n";
    std::cout << "| D E A T H   D E A T H   D E A T H   D E A T H  +*#@@@@@#+-:      ..::--:.            .-=*+:     E A T H   D E A T H   D E A T H   D E A T H   D E A|\n";
    std::cout << "|   D E A T H   D E A T H   D E A T H   D E A T  +@@@#=:             +#**=-.               :==-   D E A T H   D E A T H   D E A T H   D E A T H   D E|\n";
    std::cout << "| H   D E A T H   D E A T H   D E A T H   D E A  #%=.                -%@@%+                  -*:    D E A T H   D E A T H   D E A T H   D E A T H   D|\n";
    std::cout << "| T H   D E A T H   D E A T H   D E A T H   D E  @#                  +@@@%+                  -#.  H   D E A T H   D E A T H   D E A T H   D E A T H  |\n";
    std::cout << "| A T H   D E A T H   D E A T H   D E A T H   D  %@-                -@@@@#=.          ..  .  *@-  T H   D E A T H   D E A T H   D E A T H   D E A T H|\n";
    std::cout << "| E A T H   D E A T H   D E A T H   D E A T H   -#@%:           . .*@@@*-:.=          ..::. .@*.  A T H   D E A T H   D E A T H   D E A T H   D E A T|\n";
    std::cout << "| D E A T H   D E A T H   D E A T H   D E A T H =%@@@=.       .:=*@@@%-     :             .:##:   E A T H   D E A T H   D E A T H   D E A T H   D E A|\n";
    std::cout << "|   D E A T H   D E A T H   D E A T H   D E A T *@@@%%@@@@%*#%%%@@@@@-      .- .-=*######%##+:.   D E A T H   D E A T H   D E A T H   D E A T H   D E|\n";
    std::cout << "| H   D E A T H   D E A T H   D E A T H   D E A #@@#+++=-:.:::-+@@@%+        :. ::-=-====--:..      D E A T H   D E A T H   D E A T H   D E A T H   D|\n";
    std::cout << "| T H   D E A T H   D E A T H   D E A T H   D E :###%%#+--=+++#%@@@*         .=   .--+++==+=-: .  H   D E A T H   D E A T H   D E A T H   D E A T H  |\n";
    std::cout << "| A T H   D E A T H   D E A T H   D E A T H   D   .=:.   .:-+%@@@@%+    .    :-   .:-:::-::...    T H   D E A T H   D E A T H   D E A T H   D E A T H|\n";
    std::cout << "| E A T H   D E A T H   D E A T H   D E A T H         -.     +@@@@%%-  .-.   +-   .             E A T H   D E A T H   D E A T H   D E A T H   D E A T|\n";
    std::cout << "| D E A T H   D E A T H   D E A T H   D E A T H       --.    #@@@@@%#+=+#: .:.  .:.   E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D E A|\n";
    std::cout << "|   D E A T H   D E A T H   D E A T H   D E A T H     +++:  =@@@@@@@@%@@@+++**==-=:   D E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D E|\n";
    std::cout << "| H   D E A T H   D E A T H   D E A T H   D E A T H   *%+*. %@@@@@@@@@@@@@#%%@*=+==.    D E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D|\n";
    std::cout << "| T H   D E A T H   D E A T H   D E A T H   D E A T  .%@##*+#%%@@%@*@@%@%@%#%%%=-=..  H   D E A T H   D E A T H   D E A T H   D E A T H   D E A T H  |\n";
    std::cout << "| A T H   D E A T H   D E A T H   D E A T H   D E A  .%%%@@@*+*+-**:+:-*-:+-::.:      T H   D E A T H   D E A T H   D E A T H   D E A T H   D E A T H|\n";
    std::cout << "| E A T H   D E A T H   D E A T H   D E A T H   D E   =#@@@%#+:*=@@=@#*@#=@*=*:+.   E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D E A T|\n";
    std::cout << "| D E A T H   D E A T H   D E A T H   D E A T H   D E  :#@@@%#-::+=.+=:+::=-.. .    D E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D E A|\n";
    std::cout << "|   D E A T H   D E A T H   D E A T H   D E A T H   D   :@@@@@*=-=-:-... ..       H   D E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D E|\n";
    std::cout << "| H   D E A T H   D E A T H   D E A T H   D E A T H   D  -@@@@%#*+#**#***+-:::.   T H   D E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D|\n";
    std::cout << "| T H   D E A T H   D E A T H   D E A T H   D E A T H     :*%%%@@%*#*#*=-:.:.       T H   D E A T H   D E A T H   D E A T H   D E A T H   D E A T H  |\n";
    std::cout << "| A T H   D E A T H   D E A T H   D E A T H   D E A T H     :%@@@@@@@%%%#*+=--=::.  A T H   D E A T H   D E A T H   D E A T H   D E A T H   D E A T H|\n";
    std::cout << "| E A T H   D E A T H   D E A T H   D E A T H   D E A T H    .*@@@@@@@@@%#*++++=-:  E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D E A T|\n";
    std::cout << "| D E A T H   D E A T H   D E A T H   D E A T H   D E A T H    -%@@@@@%%#*+++++=:   D E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D E A|\n";
    std::cout << "|   D E A T H   D E A T H   D E A T H   D E A T H   D E A T H    .:--:..  ....        D E A T H   D E A T H   D E A T H   D E A T H   D E A T H   D E|\n";
    std::cout << "+----------------------------------------------------------------------------------------------------------------------------------------------------+";
}

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
                Grid[h][w] = 9;
            }
            else{
                if(Grid[h][w] == 9){
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
    int tempnum = 0;
    while(!_kbhit()){
        clearConsole();
        std::cout << "+----------------------------------------------------------------------------------------------------------------------------------------------------+\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|   If you cannot see the entire rectangle,                                                                                                          |\n";
        std::cout << "|   set good font size in command line settings.                                                                                                     |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|   More info on my github:                                                                                                                          |\n";
        std::cout << "|   github.com/Nixhekoo/NTR-NixhekoosTerminalRaycaster_CPP                                                                                           |\n";
        std::cout << "|   (You can copy the link using cursor.)                                                                                                            |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|   Press SPACE to continue to the Game                                                                                                              |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                                    |\n";
        std::cout << "|                                                                                                                                         D 3 4 7 H  |\n";
        std::cout << "+----------------------------------------------------------------------------------------------------------------------------------------------------+";
        if(tempnum < 40){
            tempnum++;
            Sleep(100);
        } else {
            tempnum = 0;
            system("cls");
        }
    }
    system("cls");
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
                if(w == 0 || w == ScreenWIDTH - 1){
                    std::cout << "+";
                } else {
                    std::cout << "-";
                }
            } else if(w == 0 || w == ScreenWIDTH - 1){
                std::cout << "|";
            } else {
                std::cout << Screen[h][w];
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\nPlayer Height Position: " << playerPosH << "  Player Width Position: " << playerPosW << "  Player Direction: " << playerDir << "                 \n\nKeybinds:\n[WASD] = Move\n[JL] = Rotate\n[X] = Clear Screen (If any bugs appear outside main 'Rectangle')";
}

void singleRay(double rayNum){
    // Ray Variables
    double rayDirection = ((playerDir - (FOV / 2)) + rayNum);
    double rayPosH = playerPosH;
    double rayPosW = playerPosW;
    int intRayPosH;
    int intRayPosW;
    rayDistanceToWall = 0;
    bool stopCastingRay = false;
    double raySpeed = 0.15;

    // Ray Cast
    while(!stopCastingRay){
        rayPosH += raySpeed * sin(rayDirection * (MATH_PI / 180));
        rayPosW += raySpeed * cos(rayDirection * (MATH_PI / 180));
        intRayPosH = round(rayPosH);
        intRayPosW = round(rayPosW);
        if(Grid[intRayPosH][intRayPosW] == 1){
            stopCastingRay = true;
            rayTouchingBlockType = 1;
        } else if(Grid[intRayPosH][intRayPosW] == 2){
            stopCastingRay = true;
            rayTouchingBlockType = 2;
        } else {
            rayDistanceToWall += raySpeed;
            fixedRayDistanceToWall = rayDistanceToWall * cos((rayDirection - playerDir) * (MATH_PI / 180));
            if(fixedRayDistanceToWall > renderDistance + 2){
                stopCastingRay = true;
                rayTouchingBlockType = 0;
            }
        }
    }
}

void raycast(){
    if(playerPosW < -4001.5){
        deathdimensionTimer++;
        if(deathdimensionTimer == 20){
            deathdimensionTimer = 0;
        }
    } else {
        deathdimensionTimer = -1;
    }
    if(deathdimensionTimer % 10 == 0){
        deathDimensionAccessed();
        Sleep(500);
    } else {
        for(double i = 0; i < FOV; i += 0.65){
            singleRay(i);
            int selectedScreenWidth = (ScreenWIDTH / FOV) * i;
            int startingCharHeight = (ScreenHEIGHT / 2) - (wallHeight / fixedRayDistanceToWall) * distanceToProjectionPlane;
            int endingCharHeight = (ScreenHEIGHT / 2) + (wallHeight / fixedRayDistanceToWall) * distanceToProjectionPlane;
            for(int j = 0; j < ScreenHEIGHT; j++){
                if(j >= startingCharHeight && j <= endingCharHeight){
                    if(rayTouchingBlockType == 1){
                        if(playerPosW > -401.5){
                            if(playerPosW > -1.5){
                                if(round(fixedRayDistanceToWall) <= 0){
                                    Screen[j][selectedScreenWidth] = '@';
                                } else if(round(fixedRayDistanceToWall) <= round(renderDistance * 0.20)){
                                    Screen[j][selectedScreenWidth] = '#';
                                } else if(round(fixedRayDistanceToWall) <= round(renderDistance * 0.35)){
                                    Screen[j][selectedScreenWidth] = '%';
                                } else if(round(fixedRayDistanceToWall) <= round(renderDistance * 0.5)){
                                    Screen[j][selectedScreenWidth] = '*';
                                } else if(round(fixedRayDistanceToWall) <= round(renderDistance * 0.65)){
                                    Screen[j][selectedScreenWidth] = '+';
                                } else if(round(fixedRayDistanceToWall) <= round(renderDistance * 0.8)){
                                    Screen[j][selectedScreenWidth] = '-';
                                } else if(round(fixedRayDistanceToWall) <= renderDistance){
                                    Screen[j][selectedScreenWidth] = '.';
                                } else if(round(fixedRayDistanceToWall) > renderDistance){
                                    Screen[j][selectedScreenWidth] = ' ';
                                }
                            } else {
                                Screen[j][selectedScreenWidth] = limbodimensionText[selectedScreenWidth % limbodimensionText.length()];
                            }
                        } else {
                            Screen[j][selectedScreenWidth] = ' ';
                        }
                    } else if(rayTouchingBlockType == 2){
                        Screen[j][selectedScreenWidth] = doorText[selectedScreenWidth % doorText.length()];
                    } else if(rayTouchingBlockType == 0){
                        Screen[j][selectedScreenWidth] = ' ';
                    }
                } else {
                    if(playerPosW > -4001.5){
                        if(playerPosW > -1001.5){
                            if(playerPosW > -401.5){
                                Screen[j][selectedScreenWidth] = ' ';
                            } else {
                                Screen[j][selectedScreenWidth] = undefineddimensionText[selectedScreenWidth % undefineddimensionText.length()];
                            }
                        } else {
                            Screen[j][selectedScreenWidth] = returndimensionText[selectedScreenWidth % returndimensionText.length()];
                        }
                    } else {
                        Screen[j][selectedScreenWidth] = deathdimensionText[selectedScreenWidth % deathdimensionText.length()];
                    }
                }
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