#pragma once
#include "CommonFunc.h"
#include "LTexture.h"

class GetPlayerName {
private:
    bool isHadNameInput;
    string inputText;
    SDL_Event e;
    //Luu man hinh chinh cua GetPlayerName
    LTexture GPNTexture;
    LTexture gInputTextTexture;

public:
    GetPlayerName();
    bool LoadMedia();
    void render();
    bool HandleEvent();
    bool run();
    string getInputText();
    ~GetPlayerName();
};