#include "Main.h"

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {    
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);


    MyGame gameEngine(hInstance, 1920, 1080);

    return gameEngine.run();
}