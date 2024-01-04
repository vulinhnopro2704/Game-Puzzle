#include "MenuStart.h"
#include "Gameplay.h"
bool isBack = false;
LTexture SButtonMode;
SDL_Rect SButtonModeRect[] = { {0, 0, 186, 58}, {0, 70, 186, 58}, {0, 140, 186, 58}, {0, 210, 186, 58} };
LTexture UnfinishedTexture;

MenuStart::MenuStart() : isRunning(true), MenuStartButton(vector<LTexture>(6)), LargeImage(nullptr), MenuImage(vector<LTexture>(TOTAL_IMAGE)){
    isChooseMode = false;
    isOut = false;
}

MenuStart::MenuStart(string PlayerName) : isRunning(true), MenuStartButton(vector<LTexture>(6)), LargeImage(nullptr), MenuImage(vector<LTexture>(TOTAL_IMAGE))
{  
    isChooseMode = false;
    isOut = false;
    this->PlayerName = PlayerName;
}

MenuStart::~MenuStart() {
    close();
}

LTexture TypeMenu;

std::vector<LTexture> TmpImages(TOTAL_IMAGE);

bool MenuStart::loadMedia() {
    bool success = true;

    if (!HTexture.loadFromFile("IMG//Start_Menu//ModeAndImage.PNG", false)) {
        std::cout << "Can't not load ModeAndImage.PNG : " << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_MODE_3].loadFromFile("IMG//Start_Menu//3x3.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_MODE_4].loadFromFile("IMG//Start_Menu//4x4.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_MODE_5].loadFromFile("IMG//Start_Menu//5x5.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_MODE_6].loadFromFile("IMG//Start_Menu//6x6.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_TURN_LEFT].loadFromFile("IMG//Start_Menu//TurnLeft.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else if (!MenuStartButton[BUTTON_TURN_RIGHT].loadFromFile("IMG//Start_Menu//TurnRight.png")) {
        std::cout << "Can't not load Mode img : \n" << IMG_GetError();
        success = false;
    }
    else if (!SolveMode.loadFromFile("IMG//Start_Menu//SolveMode.png"))
    {
        std::cout << "Can't not load Solve Mode IMG: " << IMG_GetError() << std::endl;
        success = false;
    }
    else if (!SButtonMode.loadFromFile("IMG//Mode.png"))
    {
        success = false;
    }
    else if (!TypeMenu.loadFromFile("IMG//Start_Menu//TypeMenu.png", false))
    {
        std::cout << "Can't not load Type Menu IMG: " << IMG_GetError() << std::endl;
        success = false;
    }
    else if (!PlayerNameTexture.loadFromRenderedText(PlayerName, { 0xFF, 0xFF, 0xFF, 0xFF }, 30))
    {
        std::cout << "Can't not load PlayerName Texture: " << IMG_GetError() << std::endl;
        success = false;
    }
    else if (!UnfinishedTexture.loadFromFile("IMG//Start_Menu//Unfinished Challenge.png", false))
    {
        std::cout << "Can't not load Unfinished Challenge Texture: " << IMG_GetError() << std::endl;
        success = false;
    }
    else 
    {
        for (int i = 0; i < TOTAL_IMAGE; ++i)
        {
            const std::string index = std::to_string(i);
            if (!MenuImage[i].loadFromFile("Data//MenuImage//MenuImage" + index + ".png"))
            {
                std::cout << "Can't not load MenuImage img : \n" << IMG_GetError();
                success = false;
            }
            else
                TmpImages[i] = MenuImage[i];
        }
    }
    
    for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
        gSpriteClips[i].x = 12;
        gSpriteClips[i].y = 10 + 144 * i;
        gSpriteClips[i].w = 144;
        gSpriteClips[i].h = 140;
    }
    for (int i = 0; i < 4; ++i)
    {
        gButton[i].SetAllValue(568 + i * 181, 537, 144, 144);
    }
    for (int i = 4; i < START_BUTTON_TOTAL; ++i)
    {
        gButton[i].SetAllValue(617 + (i - 4) * 461, 285, 144, 144);
    }
    MenuImage[0].render(820, 256);
    return success;
}

void MenuStart::close() {
    HTexture.free(); // Giải phóng texture
    for (int i = 0; i < START_BUTTON_TOTAL; ++i) {
        MenuStartButton[i].free();
    }

    // Giải phóng tất cả các MenuImage đã được tạo
    for (int i = 0; i < TOTAL_IMAGE; ++i) {
        MenuImage[i].free();
    }
    // Không cần giải phóng renderer và window ở đây vì chúng được quản lý trong MainMenu.

    // Đóng SDL_ttf và Mix
    //TTF_Quit();
    //Mix_Quit();
}

bool MenuStart::CheckNonFinishGame(int n)
{
    string tenfile = "ContinueGame//puzzle";
    string N = to_string(n);
    tenfile += N + ".txt";
    ifstream inFile(tenfile);
    bool status;
    inFile >> status;
    return status;
}

void MenuStart::run() {
    if (outGame) return;
    if (!loadMedia())
    {
        std::cout << "Fail to Load Media \n";
    }
    else
    {
        SDL_Event e;
        isRunning = true;
        while (isRunning && !outGame) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    isRunning = false;
                    outGame = true;
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                {
                    isRunning = true;
                    outGame = true;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    cout << e.motion.x << " " << e.motion.y << endl;
                    if (e.motion.x >= 1190 && e.motion.x <= 1274 && e.motion.y <= 79)
                    {
                        isRunning = false;
                        isOut = true;
                        return;
                    }
                }
                for (int i = 0; i < START_BUTTON_TOTAL; ++i) {
                    gButton[i].HandleEvent(&e);
                    // Xác định nút nào được nhấn và thực hiện tác vụ tương ứng
                    if (e.type == SDL_MOUSEBUTTONDOWN && gButton[i].getCurrentSprite() == BUTTON_SPRITE_MOUSE_DOWN) {
                        switch (i) {
                        case BUTTON_MODE_3:
                        {
                            //Ô 3 x 3
                            n = 3;
                            isBack = false;
                            isRunning = false;
                            break;
                        }
                        case BUTTON_MODE_4:
                        {
                            //Ô 4 x 4
                            n = 4;                          
                            isRunning = false;
                            isBack = false;
                            break;
                        }
                        case BUTTON_MODE_5:
                        {
                            //Ô 5 x 5
                            n = 5;                           
                            isRunning = false;
                            isBack = false;
                            break;
                        }
                        case BUTTON_MODE_6:
                        {
                            //Ô 6 x 6
                            n = 6;                           
                            isRunning = false;
                            isBack = false;
                            break;
                        }
                        case BUTTON_TURN_LEFT:
                        {
                            //Chuyển ảnh trái
                            if (Order == 0) 
                                Order = TOTAL_IMAGE - 1;
                            else --Order;
                            break;
                        }
                        case BUTTON_TURN_RIGHT:
                        {
                            //Chuyển ảnh phải
                            ++Order;
                            break;
                        }
                        default:
                            break;
                        }
                    }
                }
            }
            /*SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);*/
            TmpImages[Order % TOTAL_IMAGE].Resize(208, 208);
            HTexture.render(0, 0);
            PlayerNameTexture.render(286 - PlayerNameTexture.getWidth() / 2, 253);
            for (int i = 0; i < START_BUTTON_TOTAL; ++i)
            {
                gButton[i].render(MenuStartButton[i], gSpriteClips);
            }
            TmpImages[Order % TOTAL_IMAGE].render(820, 256);
            if (!isRunning && isChooseMode == false && !outGame && isBack == false)
            {
                SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

                // Đặt màu vẽ là đen (0, 0, 0, 200) - mờ
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 180);

                // Tạo một SDL_Rect có kích thước như bạn muốn
                SDL_Rect FillRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

                // Vẽ hình chữ nhật mờ lên Renderer
                SDL_RenderFillRect(gRenderer, &FillRect);
                // Đặt lại chế độ blend về mặc định
                SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
                if (!outGame) HandleEvent();
            }
            SDL_RenderPresent(gRenderer);
        }
    }
}

void MenuStart::HandleEvent2()
{
    UnfinishedTexture.render(0, 0);
    SDL_RenderPresent(gRenderer);
    bool isQuit = false;
    SDL_Event e;
    while (!isQuit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                isQuit = true;
                outGame = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.motion.y >= 471 && e.motion.y <= 537)
                {
                    if (e.motion.x >= 397 && e.motion.x <= 600)
                    {
                        Unfinished = true;
                        isQuit = true;
                        isRunning = false;
                        return;
                    }
                    else if (e.motion.x >= 688 && e.motion.x <= 888)
                    {
                        isQuit = true;
                    }
                }
            }
        }
    }
    HandleEvent1();
}

void MenuStart::HandleEvent1()
{
    TypeMenu.render(0, 0);
    SButtonMode.render(515, 73, &SButtonModeRect[n - 3]);
    SDL_RenderPresent(gRenderer);
    bool isQuitType = false;
    isChooseMode = false;
    Mode = 0;
    SDL_Event e;
    /*1021 98
    1022 206
        964 155
        1086 158*/
    while (!isQuitType)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                isQuitType = true;
                outGame = true;
                if (Mode == 0)
                {
                    isChooseMode = false;
                }
                break;
            }
            else
            {
                if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    cout << e.motion.x << " " << e.motion.y << endl;
                    if (e.motion.y >= 312 && e.motion.y <= 597)
                    {
                        if (e.motion.x >= 248 && e.motion.x <= 600)
                        {
                            isChooseMode = true;
                            Mode = 1;
                        }
                        else if (e.motion.x >= 712 && e.motion.x <= 1064)
                        {
                            isChooseMode = true;
                            Mode = 2;
                        }
                    }
                    else if (e.motion.x >= 964 && e.motion.x <= 1086 && e.motion.y >= 98 && e.motion.y <= 206)
                    {
                        isQuitType = true;
                        isBack = true;
                        isRunning = true;
                    }
                }
                if (Mode != 0 && isChooseMode == true) isQuitType = true;
            }
        }
    }
}

void MenuStart::HandleEvent()
{
    bool check = CheckNonFinishGame(n);
    if (!check)
    {
        HandleEvent1();
    }
    else
    {
        HandleEvent2();
    }
}
