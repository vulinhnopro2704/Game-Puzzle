#pragma once
#ifndef  START_H_
#define START_H_

#include "CommonFunc.h"
#include "LTexture.h"
#include "LButton.h"

class MenuStart {
private:
    LTexture HTexture;
    vector<LTexture> MenuStartButton;
    LTexture SolveMode;
    bool isRunning;
    bool isChooseMode;
    bool isOut;
    string PlayerName;
    bool Unfinished = false;

    enum StartButton
    {
        BUTTON_MODE_3 = 0,
        BUTTON_MODE_4 = 1,
        BUTTON_MODE_5 = 2,
        BUTTON_MODE_6 = 3,
        BUTTON_TURN_LEFT = 4,
        BUTTON_TURN_RIGHT = 5,
        START_BUTTON_TOTAL = 6
    };

    SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect gRect[START_BUTTON_TOTAL];
    LButton gButton[START_BUTTON_TOTAL];
    SDL_Texture* LargeImage;
    LTexture PlayerNameTexture;
    std::vector<LTexture> MenuImage;

public:
    MenuStart();
    MenuStart(string PlayerName);
    ~MenuStart();
    bool init();
    bool loadMedia();
    void close();
    bool CheckNonFinishGame(int n);
    void run();
    void HandleEvent2();
    void HandleEvent1();
    void HandleEvent();
    bool getIsChooseMode() { return isChooseMode; }
    bool getIsOut(){ return isOut;  }
    bool getUnfinished() { return Unfinished; }
};


#endif // ! HELP_H
