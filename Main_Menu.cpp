#include "Main_Menu.h"

bool Continue = false;

MainMenu::MainMenu() {
    mQuit = false;
    CanGetName = true;
}


MainMenu::~MainMenu() {
    //close();
}

bool MainMenu::init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("SLIDING PUZZLE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == nullptr) {
            success = false;
        }
        else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                success = false;
            }
            else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    success = false;
                }

                if (TTF_Init() == -1) {
                    success = false;
                }
                if (!(gFont = TTF_OpenFont("Font//Snacko Trial.otf", 56))) {
                    std::cout << "Can't Initialize gFont TTF_Error: " << TTF_GetError() << std::endl;
                    success = false;
                }
                
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    success = false;
                }
            }
        }
    }
    isInit = success;
    return success;
}

bool MainMenu::loadMedia() {
    bool success = true;

    if (!MTexture.loadFromFile("Data//Main Menu//Main Menu.png")) {
        success = false;
    }
    else if (!Start.loadFromFile("Data//Main Menu//Start.png")) {
        success = false;
    }
    else if (!BHelp.loadFromFile("Data//Main Menu//Help.png")) {
        success = false;
    }
    else if (!BLeaderBoard.loadFromFile("Data//Main Menu//LeaderBoard.png")) {
        success = false;
    }
    else {
        MTexture.Resize(SCREEN_WIDTH, SCREEN_HEIGHT);
        for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
            gSpriteClips[i].x = 0;
            gSpriteClips[i].y = 116 * i;
            gSpriteClips[i].w = ButtonWidth;
            gSpriteClips[i].h = ButtonHeight;
        }

        for (int i = 0; i < TOTAL_BUTTONS; ++i)
        {
            gButtons[i].SetAllValue(_X, _Y + i * (ButtonHeight + Distance), ButtonWidth, ButtonHeight);
        }
    }

    return success;
}

void MainMenu::close() {
    MTexture.free();
    MTexture.free();
    Start.free();
    BHelp.free();
    BLeaderBoard.free();
}

void MainMenu::run() {
    if (outGame) 
        return;
    if (!isInit)
    {
        if (!init()) {
            return;
        }
    }
    if (!loadMedia()) {
        return;
    }
    bool isBackButtonClick = false;
    SDL_Event e;
    while (!mQuit && !outGame) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                mQuit = true;
                outGame = true;
                isBackButtonClick = false;
            }

            for (int i = 0; i < TOTAL_BUTTONS; ++i) {
                gButtons[i].HandleEvent(&e);
                // Xác định nút nào được nhấn và thực hiện tác vụ tương ứng
                if (isBackButtonClick || e.type == SDL_MOUSEBUTTONDOWN && gButtons[i].getCurrentSprite() == BUTTON_SPRITE_MOUSE_DOWN) {
                    if (isBackButtonClick && !outGame) i = BUTTON_START;
                    switch (i) {
                    case BUTTON_START:
                    {
                        GetPlayerName GetPlayerName;
                        if (CanGetName)
                        {
                            if (!GetPlayerName.run())
                            {
                                cout << "Player Name is Empty " << endl;
                                break;
                            }
                            else
                            {
                                CanGetName = false;
                                PlayerName = GetPlayerName.getInputText();
                            }
                        }
                        isBackButtonClick = false;
                        // Thực hiện hành động khi nút PLAY_BUTTON được nhấn
                        MenuStart startgame(PlayerName);
                        do { 
                            startgame.run();
                            Continue = startgame.getUnfinished();
                            CanGetName = startgame.getIsOut();
                        } while (!startgame.getIsChooseMode() && !outGame && !startgame.getIsOut() && !Continue);
                        if (!outGame && !startgame.getIsOut())
                        {
                            Gameplay* game;
                            game = new Gameplay(PlayerName, startgame.getUnfinished());
                            game->Run();
                            if (!outGame) isBackButtonClick = true;
                        }
                        else if (startgame.getIsOut())
                        {
                            break;
                        }
                        else
                        {
                            mQuit = true;
                        }
                        break;
                    }
                    case BUTTON_HELP:
                        // Thực hiện hành động khi nút INSTRUCTIONS_BUTTON được nhấn
                        //Chuyển đến màn hình hướng dẫn
                        if (!outGame)
                        {
                            Help gamehelp;
                            gamehelp.run();
                            if (gamehelp.getIsBackHelp())
                            {
                                outGame = false;
                                SDL_Delay(100);
                                while (SDL_PollEvent(&e));
                                break;
                            }
                        }
                        break;
                    case BUTTON_LEADERBOARD:
                        // Thực hiện hành động khi nút EXIT_BUTTON được nhấn
                        LeaderBoard HighScore;
                        HighScore.run();
                    }
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                {
                    mQuit = true;
                    outGame = true;
                }
            }
        }
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        if (!outGame)
        {
            MTexture.render(0, 0);
            gButtons[BUTTON_START].render(Start, gSpriteClips);
            gButtons[BUTTON_HELP].render(BHelp, gSpriteClips);
            gButtons[BUTTON_LEADERBOARD].render(BLeaderBoard, gSpriteClips);
            SDL_RenderPresent(gRenderer);
        }
    }

    close();
}