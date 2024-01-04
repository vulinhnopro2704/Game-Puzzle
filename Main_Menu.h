#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H

#include "CommonFunc.h"
#include "LTexture.h"
#include "LButton.h"
#include "Help.h"
#include "MenuStart.h"
#include "LeaderBoard.h"
#include "GetPlayerName.h"

class MainMenu {
public:
    MainMenu();
    ~MainMenu();
    bool init();
    bool loadMedia();
    void close();
    void run();

private:
    //Các thông số kích thước, tọa độ nút:
/*    Khoảng cách giữa các nút : 10px
        Số nút : 3
        Tính toán tọa độ và kích thước chung :
    ### Tọa Độ X mỗi nút :
    X = SCREEN_WIDTH / 3
        ###Tọa độ Y mỗi nút :
    Y = SCREEN_HEIGHT / 2 + (nút_i * (Chiều Cao mỗi nút + Khoảng cách giữa các nút))
        Chiều rộng mỗi nút(Start, Help, Leader Board) của menu chính :
    SCREEN_WIDTH / 3
        Chiều cao mỗi nút(Start, Help, Leader Board) của menu chính :
    (SCREEN_HEIGHT / 2 - (Số nút - 1) x Khoảng cách giữa các nút) / Số nút
    (SCREEN_HEIGHT / 2 - 2 * 20) / 3
*/
    //Sprite Distance
    const int SpriteDistance = 116;
    //Khoảng cách giữa các nút
    const int Distance = 10;
    //Tọa độ X của các nút
    const int _X = SCREEN_WIDTH / 3;
    //Tọa độ Y ban đầu của các nút
    const int _Y = SCREEN_HEIGHT / 2;
    //Chiều rộng môi nút
    const int ButtonWidth = SCREEN_WIDTH / 3;
    //Chiều cao mỗi nút
    const int ButtonHeight = (SCREEN_HEIGHT / 2 - (TOTAL_BUTTONS - 1) * Distance) / TOTAL_BUTTONS;

    TTF_Font* gFont;
    SDL_Color mTextColor;

    LTexture MTexture; //Background
    LTexture Start; //Start Button
    LTexture BHelp; //Help Button
    LTexture BLeaderBoard; //Exit Button

    LButton gButtons[TOTAL_BUTTONS];

    bool mQuit;
    bool CanGetName;

    enum MenuButtons {
        BUTTON_START = 0,
        BUTTON_HELP = 1,
        BUTTON_LEADERBOARD = 2,
        TOTAL_BUTTONS
    };

    //Tên Người chơi
    string PlayerName;

    SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

    void renderText(const std::string& text, int x, int y);

};
#endif // MAINMENU_H
