#include "Login.h"

bool Login::Init()
{
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

void Login::TaoFile(string num, string type)
{
    string tenfile = "ContinueGame//puzzle";
    tenfile += type + num + ".txt";
    ofstream taofile(tenfile);
    taofile << 0 << endl;
    taofile.close();
}

int Login::getNumFile()
{
    return Account[Name].second;
}

Login::Login()
{
    ifstream inFile("Account//Account.txt");
    if (inFile.is_open())
    {
        inFile >> CountAccount;
        for (int i = 0; i < CountAccount; i++)
        {
            string name;
            inFile >> name;
            CheckA[name] = true;
        }
        for (int i = 0; i < CountAccount; i++)
        {
            string name;
            inFile >> name;
            inFile >> Account[name].first >> Account[name].second;
        }
        inFile.close();
    }
    cout << CountAccount << endl;
}

Login::~Login()
{
    ofstream outFile("Account//Account.txt");
    if (outFile.is_open())
    {
        outFile << CountAccount << endl;
        for (auto x: CheckA)
        {
            outFile << x.first << " ";
        }
        outFile << endl;
        for (auto x : Account)
        {
            outFile << x.first << " " << x.second.first << " " << x.second.second << endl;
        }
        outFile.close();
    }
}

bool Login::Register()
{
    
    return false;
}

bool Login::login()
{

    return false;
}

bool Login::LoadFromFile()
{
    if (!Llogin.loadFromFile("IMG//Login//Login.png")) return false;
    else if (!Lregister.loadFromFile("IMG//Login//Register.png")) return false;
    return true;
}

bool Login::HandleEvent()
{
    //cout << "Handle Event " << endl;
    //Main loop flag
    SDL_Event e;
    //Set text color as black
    SDL_Color textColor = { 0, 0, 0, 0xFF };

    //The current input text.
    LNinputTextTexture.loadFromRenderedText(Name.c_str(), textColor);
    LPinputTextTexture.loadFromRenderedText(Pass.c_str(), textColor);
    RNinputTextTexture.loadFromRenderedText(RName.c_str(), textColor);
    RPinputTextTexture.loadFromRenderedText(RPass.c_str(), textColor);
    RCPinputTextTexture.loadFromRenderedText(RCPass.c_str(), textColor);
    //Enable text input
    SDL_StartTextInput();
    //While application is running
    while (!Lquit)
    {
        //The rerender text flag
        bool renderText = false;

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                Lquit = true;
                outGame = true;
                break;
            }
            //Special key input
            if (Log)
            {
                if (e.type == SDL_KEYDOWN)
                {
                    //Handle backspace
                    if (e.key.keysym.sym == SDLK_BACKSPACE)
                    {
                        //lop off character
                        if (Lcheck)
                        {
                            if (Name.size()) Name.pop_back();
                        }
                        else
                        {
                            if (Pass.size())
                            {
                                Pass.pop_back();
                                HidePass.pop_back();
                            }

                        }
                        renderText = true;
                    }
                    //Handle copy
                    else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
                    {
                        if (Lcheck) SDL_SetClipboardText(Name.c_str());
                        else SDL_SetClipboardText(Pass.c_str());
                    }
                    //Handle paste
                    else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
                    {
                        if (Lcheck) Name = SDL_GetClipboardText();
                        else Pass = SDL_GetClipboardText();
                        renderText = true;
                    }
                    else if ((e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_RETURN2))
                    {
                        if (Name.size())
                        {
                            if (!Lcheck)
                            {
                                if (Pass.size())
                                {
                                    if (CheckA[Name])
                                    {
                                        if (Pass == Account[Name].first)
                                        {
                                            cout << "Dang nhap thanh cong\n";
                                            Lquit = true;
                                        }
                                        else
                                        {
                                            cout << "Sai mat khau, vui long nhap lai!\n";

                                        }
                                    }
                                    else
                                    {
                                        CheckA.erase(Name);
                                        cout << "Tai khoan khong ton tai!\n";
                                    }
                                }
                                else
                                {
                                    cout << "Vui long nhap mat khau!\n";
                                }
                                
                                
                            }
                            else
                            {
                                Lcheck = false;
                            }
                        }
                        //isHadNameInput = true;
                    }
                }
                //Special text input event
                else if (e.type == SDL_TEXTINPUT)
                {
                    if (Lcheck)
                    {
                        if (Name.size() <= 16 && !(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                        {
                            //Append character
                            Name += e.text.text;
                            renderText = true;
                        }
                    }
                    else
                    {
                        if (Pass.size() <= 16 && !(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                        {
                            //Append character
                            Pass += e.text.text;
                            HidePass += '*';
                            renderText = true;
                        }
                    }

                }
                else if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    int x = e.motion.x, y = e.motion.y;
                    cout << x << " " << y << endl;
                    if (x >= 977 && x <= 977 + 115 && y >= 111 && y <= 226)
                    {
                        Lquit = true;
                        //isHadNameInput = false;
                        break;
                    }
                    //else if (x >= 407 && x <= 407 + 470 && y >= 509 && y <= 613 && Name.size() && Pass.size())
                    //{
                    //    Lquit = true;
                    //    //isHadNameInput = true;
                    //}
                    else if (x >= 800 && x <= 840 && y >= 375 && y <= 400)
                    {
                        Hide = !Hide;
                        if (Hide) LPinputTextTexture.loadFromRenderedText(HidePass.c_str(), textColor, 25);
                        else LPinputTextTexture.loadFromRenderedText(Pass.c_str(), textColor, 25);
                    }
                    else if (x >= 770 && x <= 900 && y >= 520 && y <= 555)
                    {
                        cout << "Register\n";
                        Log = false;
                    }
                    else if (x >= 430 && x <= 850)
                    {
                        if (y >= 260 && y <= 305) Lcheck = true;
                        else if (y >= 365 && y <= 410) Lcheck = false;
                        else if (y >= 445 && y <= 500)
                        {
                            // Login
                            if (Pass.size() && Name.size())
                            {
                                if (CheckA[Name])
                                {
                                    if (Pass == Account[Name].first)
                                    {
                                        cout << "Dang nhap thanh cong\n";
                                        Lquit = true;
                                    }
                                    else
                                    {
                                        cout << "Sai mat khau, vui long nhap lai!\n";

                                    }
                                }
                                else
                                {
                                    CheckA.erase(Name);
                                    cout << "Tai khoan khong ton tai!\n";
                                }

                            }
                            else if(Name.size())
                            {
                                cout << "Vui long nhap mat khau\n";
                            }
                            else if (Pass.size())
                            {
                                cout << "Vui long nhap tai khoan\n";
                            }
                            else
                            {
                                cout << "Vui long nhap tai khoan va mat khau\n";
                            }
                        }
                    }
                }
            }
            else
            {
                //Register;
                if (e.type == SDL_KEYDOWN)
                {
                    //Handle backspace
                    if (e.key.keysym.sym == SDLK_BACKSPACE)
                    {
                        //lop off character
                        if (Rcheck == 0)
                        {
                            if (RName.size()) RName.pop_back();
                        }
                        else if(Rcheck == 1)
                        {
                            if (RPass.size())
                            {
                                RPass.pop_back();
                                RHidePass.pop_back();
                            }
                        }
                        else
                        {
                            if (RCPass.size())
                            {
                                RCPass.pop_back();
                                RCHidePass.pop_back();
                            }
                        }
                        renderText = true;
                    }
                    //Handle copy
                    else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
                    {
                        if (Rcheck == 0) SDL_SetClipboardText(RName.c_str());
                        else if(Rcheck == 1) SDL_SetClipboardText(RPass.c_str());
                        else SDL_SetClipboardText(RCPass.c_str());
                    }
                    //Handle paste
                    else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
                    {
                        if (Rcheck == 0) RName = SDL_GetClipboardText();
                        else if(Rcheck == 1) RPass = SDL_GetClipboardText();
                        else RCPass = SDL_GetClipboardText();
                        renderText = true;
                    }
                    else if ((e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_RETURN2))
                    {
                        if (RName.size())
                        {
                            if (Rcheck == 2)
                            {
                                if (RPass.size() && RCPass.size())
                                {
                                    if (CheckA[RName])
                                    {

                                        cout << "Tai khoan da ton tai!\n";
                                    }
                                    else
                                    {
                                        if (RPass != RCPass)
                                        {
                                            cout << "Mat khau khong trung khop\n";
                                            CheckA.erase(RName);
                                        }
                                        else
                                        {
                                            cout << "Tao tai khoan thanh cong!\n";
                                            CheckA[RName] = true;
                                            Account[RName] = { RPass, CountAccount };
                                            TaoFile(to_string(CountAccount), to_string(3));
                                            TaoFile(to_string(CountAccount), to_string(4));
                                            TaoFile(to_string(CountAccount), to_string(5));
                                            TaoFile(to_string(CountAccount), to_string(6));
                                            CountAccount++;
                                            Log = true;
                                        }

                                    }
                                }
                                else if (RPass.size())
                                {
                                    cout << "Vui long nhap lai mat khau!\n";
                                }
                                else
                                {
                                    cout << "Vui long nhap mat khau!\n";
                                }

                                
                            }
                            else
                            {
                                Rcheck++;
                            }
                        }
                        //isHadNameInput = true;
                    }
                }
                //Special text input event
                else if (e.type == SDL_TEXTINPUT)
                {
                    if (Rcheck == 0)
                    {
                        if (RName.size() <= 16 && !(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                        {
                            //Append character
                            RName += e.text.text;
                            renderText = true;
                        }
                    }
                    else if(Rcheck == 1)
                    {
                        if (RPass.size() <= 16 && !(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                        {
                            //Append character
                            RPass += e.text.text;
                            RHidePass += '*';
                            renderText = true;
                        }
                    }
                    else
                    {
                        if (RCPass.size() <= 16 && !(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                        {
                            //Append character
                            RCPass += e.text.text;
                            RCHidePass += '*';
                            renderText = true;
                        }
                    }

                }
                else if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    int x = e.motion.x, y = e.motion.y;
                    cout << x << " " << y << endl;
                    if (x >= 670 && x <= 800 && y >= 570 && y <= 610)
                    {
                        Log = true;
                        //isHadNameInput = false;
                        break;
                    }
                    //else if (x >= 407 && x <= 407 + 470 && y >= 509 && y <= 613 && Name.size() && Pass.size())
                    //{
                    //    Lquit = true;
                    //    //isHadNameInput = true;
                    //}
                    else if (x >= 430 && x <= 850)
                    {
                        if (y >= 210 && y <= 250) Rcheck = 0;
                        else if (y >= 315 && y <= 360) Rcheck = 1;
                        else if (y >= 425 && y <= 470) Rcheck = 2;
                        else if (y >= 505 && y <= 565)
                        {
                            // Register
                            if (RPass.size() && RName.size() && RCPass.size())
                            {
                                if (CheckA[RName])
                                {

                                    cout << "Tai khoan da ton tai!\n";
                                }
                                else
                                {
                                    if (RPass != RCPass)
                                    {
                                        cout << "Mat khau khong trung khop\n";
                                        CheckA.erase(RName);
                                    }
                                    else
                                    {
                                        cout << "Tao tai khoan thanh cong!\n";
                                        CheckA[RName] = true;
                                        Account[RName] = { RPass, CountAccount };
                                        TaoFile(to_string(CountAccount), to_string(3));
                                        TaoFile(to_string(CountAccount), to_string(4));
                                        TaoFile(to_string(CountAccount), to_string(5));
                                        TaoFile(to_string(CountAccount), to_string(6));
                                        CountAccount++;
                                        
                                        Log = true;
                                    }

                                }
                            }
                            else if (!RName.size())
                            {
                                cout << "Vui long nhap ten tai khoan muon tao\n";
                            }
                            else if (!RPass.size())
                            {
                                cout << "Vui long nhap mat khau!\n";
                            }
                            else if (!RCPass.size())
                            {
                                cout << "Vui long nhap lai mat khau!\n";
                            }
                            
                        }
                    }
                }
            }
            
        }
        //Rerender text if needed
        if (renderText)
        {
            //Text is not empty
            if (Log)
            {
                if (Lcheck)
                {
                    if (Name != "")
                    {
                        //Render new text
                        LNinputTextTexture.loadFromRenderedText(Name, textColor, 25);
                    }
                    //Text is empty
                    else
                    {
                        //Render space texture
                        LNinputTextTexture.loadFromRenderedText(" ", textColor, 25);
                    }
                }
                else
                {
                    if (Pass != "")
                    {
                        //Render new text
                        if (Hide) LPinputTextTexture.loadFromRenderedText(HidePass.c_str(), textColor, 25);
                        else LPinputTextTexture.loadFromRenderedText(Pass, textColor, 25);
                    }
                    //Text is empty
                    else
                    {
                        //Render space texture
                        LPinputTextTexture.loadFromRenderedText(" ", textColor, 25);
                    }
                }
            }
            else
            {
                //Register
                if (Rcheck == 0)
                {
                    if (RName != "")
                    {
                        //Render new text
                        RNinputTextTexture.loadFromRenderedText(RName, textColor, 25);
                    }
                    //Text is empty
                    else
                    {
                        //Render space texture
                        RNinputTextTexture.loadFromRenderedText(" ", textColor, 25);
                    }
                }
                else if(Rcheck == 1)
                {
                    if (RPass != "")
                    {
                        //Render new text
                        RPinputTextTexture.loadFromRenderedText(RHidePass, textColor, 25);
                        /*if (Hide) LPinputTextTexture.loadFromRenderedText(HidePass.c_str(), textColor, 25);
                        else LPinputTextTexture.loadFromRenderedText(Pass, textColor, 25);*/
                    }
                    //Text is empty
                    else
                    {
                        //Render space texture
                        RPinputTextTexture.loadFromRenderedText(" ", textColor, 25);
                    }
                }
                else
                {
                    if (RCPass != "")
                    {
                        //Render new text
                        RCPinputTextTexture.loadFromRenderedText(RCHidePass, textColor, 25);
                        /*if (Hide) LPinputTextTexture.loadFromRenderedText(HidePass.c_str(), textColor, 25);
                        else LPinputTextTexture.loadFromRenderedText(Pass, textColor, 25);*/
                    }
                    //Text is empty
                    else
                    {
                        //Render space texture
                        RCPinputTextTexture.loadFromRenderedText(" ", textColor, 25);
                    }
                }
            }
            
            
        }
        Render();
    }
    //Disable text input
    SDL_StopTextInput();
    cout << Name << " " << Pass << endl;
    cout << RName << " " << RPass << endl;
    return true;
}

void Login::Render()
{
    SDL_RenderClear(gRenderer);
    if (Log)
    {
        Llogin.render((SCREEN_WIDTH - Llogin.getWidth()) / 2, (SCREEN_HEIGHT - Llogin.getHeight()) / 2);
        LNinputTextTexture.render(450, 273);
        LPinputTextTexture.render(450, 378);
    }

    else
    {
        Lregister.render((SCREEN_WIDTH - Llogin.getWidth()) / 2, (SCREEN_HEIGHT - Lregister.getHeight()) / 2);
        RNinputTextTexture.render(450, 220);
        RPinputTextTexture.render(450, 325);
        RCPinputTextTexture.render(450, 435);
    }
    SDL_RenderPresent(gRenderer);
}

void Login::Run()
{
    if (!Init()) return;
    if (!LoadFromFile())
        return;
    SDL_Event e;
    while (!Lquit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) Lquit = true;
            else
            {
                Render();
                HandleEvent();
            }
        }
    }
}

string Login::getName()
{
    return Name;
}

