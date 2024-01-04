////
//// Created by Tuan on 8/27/2023.
////
/*
#include "Game.h"
#include "SDL_image.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Gameplay.h"
#include <string>
using namespace std;

Game::Game(int size) : n(size), p(vector<pair<int, int>>(size* size + 1))
{

};

Game::~Game() {};

GameObject** Number;

SDL_Event Game::event;
SDL_Texture *PlayerTex;
SDL_Rect srcR, destR;
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << "Subsystems Initialised!..." << endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            cout << "Window created!" << endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            cout << "Renderer created!" << endl;
        }

        isRunning = true;
    }
    else {
        isRunning = false;
    }
    game = new Gameplay(n);
    game->Random();
    Number = new GameObject*[n * n];
    for (int i = 1; i <= n*n - 1; i++) {
        p[i] = game->getPosImg(i);
    }
    const char* s1 = "Data/Number";
    const char* s2 = ".png";
    for (int i = 1; i <= n * n - 1; i++) {
        string str = to_string(i);
        string s = s1 + str + s2;
        const char* filename = s.c_str();
        Number[i] = new GameObject(filename, renderer, p[i].first, p[i].second);
    }
    //Number[1] = new GameObject("Data/Number1.png", renderer, 0, 0);
    //Number[2] = new GameObject("Data/Number2.png", renderer, 256, 0);
    //Number[3] = new GameObject("Data/Number3.png", renderer, 256*2, 0);
    //Number[4] = new GameObject("Data/Number4.png", renderer, 0, 256);
    //Number[5] = new GameObject("Data/Number5.png", renderer, 256, 256);
    //Number[6] = new GameObject("Data/Number6.png", renderer, 256 * 2, 256);
    //Number[7] = new GameObject("Data/Number7.png", renderer, 0, 256 * 2);
    //Number[8] = new GameObject("Data/Number8.png", renderer, 256, 256 * 2);
}

void Game::handleEvents() {
    
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) 
        {
            //pair<int, int> pa = game->Pos(0, game->getA());
        case SDLK_UP:
            //int x = ;
            cout << "HELLO Tuan\n";
            break;
        case SDLK_DOWN:
            //todo;
            break;
        case SDLK_LEFT:
            //todo;
            break;
        case SDLK_RIGHT:
            //todo
            break;
        default:
            break;
        }



    default:
        break;
    }
}

void Game::update() {
    for (int i = 1; i <= n * n - 1; i++)
    {
        Number[i]->Update(p[i].first, p[i].second);
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    for (int i = 1; i <= n * n - 1; i++)
    {
        Number[i]->Render();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
*/
