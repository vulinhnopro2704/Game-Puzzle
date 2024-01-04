//
// Created by ADMIN on 8/27/2023.
//
/*
#ifndef PBL2_GAMEPUZZLE_GAME_H
#define PBL2_GAMEPUZZLE_GAME_H


#include "CommonFunc.h"
#include "Gameplay.h"
using namespace std;

class Game {
public:
    Game(int n);
    ~Game();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() {
        return isRunning;
    };
    int getCnt() {
        return cnt;
    }
    int getN() {
        return n;
    }
    static SDL_Event event;
    Gameplay* game;
private:
    int cnt = 0;
    int n;
    vector<pair<int, int >> p;
    
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif //PBL2_GAMEPUZZLE_GAME_H*/
