#include "Login.h"

LTexture tmp;

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
    tmp.loadFromRenderedText("|", { 0, 0, 0 }, 25);
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
    for (auto x : CheckA) cout << x.first << " ";
    cout << endl;
    for (auto x : Account) cout << x.first << " " << x.second.first << " " << x.second.second << endl;

}

Login::~Login()
{
    cout << "out file:\n";
    ofstream outFile("Account//Account.txt");
    if (outFile.is_open())
    {
        outFile << CountAccount << endl;
        cout << "CheckA:\n";
        for (auto x: CheckA)
        {
            outFile << x.first << endl;
            cout << x.first << endl;
        }
        cout << "Account:\n";
        for (auto x : Account)
        {
            if (x.first.size() && x.second.first.size())
            {
                outFile << x.first << " " << x.second.first << " " << x.second.second << endl;
                cout << x.first << " " << x.second.first << " " << x.second.second << endl;
            }
            
        }
        outFile.close();
    }
}

void Login::RenderAnnounce(string s)
{
    Alert.render((SCREEN_WIDTH - Alert.getWidth()) / 2, (SCREEN_HEIGHT - Alert.getHeight()) / 2);
    LAnnounce.loadFromRenderedText(s, { 0xFF, 0xFF, 0xFF, 0xFF }, 20);
    LAnnounce.render((SCREEN_WIDTH - LAnnounce.getWidth()) / 2, SCREEN_HEIGHT / 2 + 36);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(1000);
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
    else if (!Alert.loadFromFile("IMG//Login//Alert.png", false))
    {
        return false;
    }
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
                RName = "|";
                RPass = " ";
                RCPass = " ";
                RHidePass = " ";
                RCHidePass = " ";
                if (e.type == SDL_KEYDOWN)
                {
                    //Handle backspace
                    if (e.key.keysym.sym == SDLK_BACKSPACE)
                    {
                        //lop off character
                        if (Lcheck)
                        {
                            if (Name.size() > 1)
                            {
                                Name.pop_back();
                            }
                        }
                        else
                        {
                            if (Pass.size() > 1)
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
                        //cout << Name << ":" << endl;
                        Name.pop_back();
                        Pass.pop_back();
                        HidePass.pop_back();
                        cout << Name << " " << Pass << " " << HidePass << endl;
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
                                            RenderAnnounce("Dang nhap thanh cong");
                                            Lquit = true;
                                        }
                                        else
                                        {
                                            RenderAnnounce("Sai mat khau, vui long nhap lai!");
                                        }
                                    }
                                    else
                                    {
                                        CheckA.erase(Name);
                                        RenderAnnounce("Tai khoan khong ton tai!");
                                    }
                                }
                                else
                                {
                                    RenderAnnounce("Vui long nhap mat khau!");
                                }
                                
                                
                            }
                            else
                            {
                                Lcheck = false;
                                ////Name.pop_back();
                                //Name += "|";
                                ////Pass.pop_back();
                                //Pass += " ";
                                ////HidePass.pop_back();
                                //HidePass += " ";
                            }
                        }
                        //isHadNameInput = true;
                        if (!Lquit)
                        {
                            Name += " ";
                            Pass += "|";
                            HidePass += "|";
                        }
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
                            if (e.text.text[0] != ' ')
                            {
                                Name.pop_back();
                                Name += e.text.text;
                                Name += "|";
                            }
                            renderText = true;
                        }
                    }
                    else
                    {
                        if (Pass.size() <= 16 && !(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                        {
                            //Append character
                            if (e.text.text[0] != ' ')
                            {
                                Pass.pop_back();
                                HidePass.pop_back();
                                Pass += e.text.text;
                                HidePass += '*';
                                /*PTrochuot.pop_back();*/
                                Pass += "|";
                                HidePass += "|";
                            }
                            renderText = true;
                        }
                    }

                }
                else if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    int x = e.motion.x, y = e.motion.y;
                    cout << x << " " << y << endl;
                    if (x >= 800 && x <= 840 && y >= 375 && y <= 400)
                    {
                        Hide = !Hide;
                        if (Hide) LPinputTextTexture.loadFromRenderedText(HidePass.c_str(), textColor, 25);
                        else LPinputTextTexture.loadFromRenderedText(Pass.c_str(), textColor, 25);
                    }
                    else if (x >= 770 && x <= 900 && y >= 520 && y <= 555)
                    {
                        cout << "Register\n";
                        Log = false;
                        Rcheck = 0;
                    }
                    else if (x >= 380 && x <= 500 && y >= 520 && y <= 560)
                    {
                        cout << "Guest\n";
                        Name = "Guest";
                        checkGuest = true;
                        string tmp = "ContinueGame//puzzle";
                        for (int i = 3; i <= 6; i++)
                        {
                            string tenfile = tmp + to_string(i) + "guest.txt";
                            cout << tenfile << endl;
                            ofstream outFile(tenfile);
                            outFile << 0;
                            outFile.close();
                        }
                        Lquit = true;
                    }
                    else if (x >= 430 && x <= 850)
                    {
                        if (y >= 260 && y <= 305)
                        {
                            Lcheck = true;
                            Name.pop_back();
                            Name += "|";
                            Pass.pop_back();
                            Pass += " ";
                            HidePass.pop_back();
                            HidePass += " ";
                        }
                        else if (y >= 365 && y <= 410)
                        {
                            Lcheck = false;
                            Name.pop_back();
                            Name += " ";
                            Pass.pop_back();
                            Pass += "|";
                            HidePass.pop_back();
                            HidePass += "|";
                        }
                        else if (y >= 445 && y <= 500)
                        {
                            // Login
                            Name.pop_back();
                            Pass.pop_back();
                            HidePass.pop_back();
                            if (Pass.size() && Name.size())
                            {
                                if (CheckA[Name])
                                {
                                    if (Pass == Account[Name].first)
                                    {
                                        RenderAnnounce("Dang nhap thanh cong");
                                        Lquit = true;
                                    }
                                    else
                                    {
                                        RenderAnnounce("Sai mat khau, vui long nhap lai!");

                                    }
                                }
                                else
                                {
                                    CheckA.erase(Name);
                                    RenderAnnounce("Tai khoan khong ton tai!");
                                }

                            }
                            else if(Name.size())
                            {
                                RenderAnnounce("Vui long nhap mat khau");
                            }
                            else if (Pass.size())
                            {
                                RenderAnnounce("Vui long nhap tai khoan");
                            }
                            else
                            {
                                RenderAnnounce("Vui long nhap tai khoan va mat khau");
                            }
                            if (!Lquit)
                            {
                                if (Lcheck)
                                {
                                    Name += "|";
                                    Pass += " ";
                                    HidePass += " ";
                                }
                                else
                                {
                                    Name += " ";
                                    Pass += "|";
                                    HidePass += "|";
                                }
                                
                            }
                        }
                    }
                }
            }
            else
            {
                Name = "|";
                Pass = " ";
                HidePass = " ";
                //Register;
                if (e.type == SDL_KEYDOWN)
                {
                    //Handle backspace
                    if (e.key.keysym.sym == SDLK_BACKSPACE)
                    {
                        //lop off character
                        if (Rcheck == 0)
                        {
                            if (RName.size() > 1) RName.pop_back();
                        }
                        else if(Rcheck == 1)
                        {
                            if (RPass.size() > 1)
                            {
                                RPass.pop_back();
                                RHidePass.pop_back();
                            }
                        }
                        else
                        {
                            if (RCPass.size() > 1)
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
                        RName.pop_back();
                        RPass.pop_back();
                        RCPass.pop_back();
                        RHidePass.pop_back();
                        RCHidePass.pop_back();
                        if (RName.size())
                        {
                            if (Rcheck == 2)
                            {
                                if (RPass.size() && RCPass.size())
                                {
                                    if (CheckA[RName])
                                    {

                                        RenderAnnounce("Tai khoan da ton tai!");
                                    }
                                    else
                                    {
                                        if (RPass != RCPass)
                                        {
                                            RenderAnnounce("Mat khau khong trung khop");
                                            CheckA.erase(RName);
                                        }
                                        else
                                        {
                                            RenderAnnounce("Tao tai khoan thanh cong!");
                                            CheckA[RName] = true;
                                            Account[RName] = { RPass, CountAccount };
                                            TaoFile(to_string(CountAccount), to_string(3));
                                            TaoFile(to_string(CountAccount), to_string(4));
                                            TaoFile(to_string(CountAccount), to_string(5));
                                            TaoFile(to_string(CountAccount), to_string(6));
                                            CountAccount++;
                                            Log = true;
                                            Lcheck = true;
                                        }

                                    }
                                }
                                else if (RPass.size())
                                {
                                    RenderAnnounce("Vui long nhap lai mat khau!");
                                }
                                else
                                {
                                    RenderAnnounce("Vui long nhap mat khau!");
                                }

                                
                            }
                            else
                            {
                                Rcheck++;
                            }
                            if (!Log)
                            {
                                if (Rcheck == 0)
                                {
                                    RName += "|";
                                    RPass += " ";
                                    RHidePass += " ";
                                    RCPass += " ";
                                    RCHidePass += " ";
                                }
                                else if (Rcheck == 1)
                                {
                                    RName += " ";
                                    RPass += "|";
                                    RHidePass += "|";
                                    RCPass += " ";
                                    RCHidePass += " ";
                                }
                                else
                                {
                                    RName += " ";
                                    RPass += " ";
                                    RHidePass += " ";
                                    RCPass += "|";
                                    RCHidePass += "|";

                                }
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
                            if (e.text.text[0] != ' ')
                            {
                                RName.pop_back();
                                RName += e.text.text;
                                RName += "|";
                            }
                            renderText = true;
                        }
                    }
                    else if(Rcheck == 1)
                    {
                        if (RPass.size() <= 16 && !(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                        {
                            //Append character
                            if (e.text.text[0] != ' ')
                            {
                                RPass.pop_back();
                                RHidePass.pop_back();
                                RPass += e.text.text;
                                RHidePass += '*';
                                RHidePass += "|";
                                RPass += "|";
                            }
                            
                            renderText = true;
                        }
                    }
                    else
                    {
                        if (RCPass.size() <= 16 && !(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                        {
                            //Append character
                            if (e.text.text[0] != ' ')
                            {
                                RCPass.pop_back();
                                RCHidePass.pop_back();
                                RCPass += e.text.text;
                                RCHidePass += '*';
                                RCPass += "|";
                                RCHidePass += "|";
                            }
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
                        Lcheck = true;
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
                        if (y >= 210 && y <= 250)
                        {
                            Rcheck = 0;
                            RName.pop_back();
                            RName += "|";
                            RPass.pop_back();
                            RCPass.pop_back();
                            RHidePass.pop_back();
                            RCHidePass.pop_back();
                            RPass += " ";
                            RCPass += " ";
                            RCHidePass += " ";
                            RHidePass += " ";

                        }
                        else if (y >= 315 && y <= 360)
                        {
                            Rcheck = 1;
                            RName.pop_back();
                            RName += " ";
                            RPass.pop_back();
                            RCPass.pop_back();
                            RHidePass.pop_back();
                            RCHidePass.pop_back();
                            RPass += "|";
                            RCPass += " ";
                            RCHidePass += " ";
                            RHidePass += "|";
                        }
                        else if (y >= 425 && y <= 470)
                        {
                            Rcheck = 2;
                            RName.pop_back();
                            RName += " ";
                            RPass.pop_back();
                            RCPass.pop_back();
                            RHidePass.pop_back();
                            RCHidePass.pop_back();
                            RPass += " ";
                            RCPass += "|";
                            RCHidePass += "|";
                            RHidePass += " ";
                        }
                        else if (y >= 505 && y <= 565)
                        {
                            // Register
                            if (RPass.size() && RName.size() && RCPass.size())
                            {
                                if (CheckA[RName])
                                {

                                    RenderAnnounce("Tai khoan da ton tai!");
                                }
                                else
                                {
                                    if (RPass != RCPass)
                                    {
                                        RenderAnnounce("Mat khau khong trung khop");
                                        CheckA.erase(RName);
                                    }
                                    else
                                    {
                                        RenderAnnounce("Tao tai khoan thanh cong!");
                                        CheckA[RName] = true;
                                        Account[RName] = { RPass, CountAccount };
                                        TaoFile(to_string(CountAccount), to_string(3));
                                        TaoFile(to_string(CountAccount), to_string(4));
                                        TaoFile(to_string(CountAccount), to_string(5));
                                        TaoFile(to_string(CountAccount), to_string(6));
                                        CountAccount++;
                                        
                                        Log = true;
                                        Lcheck = true;
                                    }

                                }
                            }
                            else if (!RName.size())
                            {
                                RenderAnnounce("Vui long nhap ten tai khoan muon tao");
                            }
                            else if (!RPass.size())
                            {
                                RenderAnnounce("Vui long nhap mat khau!");
                            }
                            else if (!RCPass.size())
                            {
                                RenderAnnounce("Vui long nhap lai mat khau!");
                            }
                            
                        }
                    }
                }
            }
            
        }
        renderText = true;
        //Rerender text if needed
        if (renderText)
        {
            //Text is not empty
            if (Log)
            {
                RName = "|";
                RPass = " ";
                RCPass = " ";
                RHidePass = " ";
                RCHidePass = " ";
                /*if (Lcheck)
                {*/
                //LNTrochuot.loadFromRenderedText(Trochuot, textColor, 25);
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
                /*}
                else
                {*/
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
                //}
            }
            else
            {
                Name = "|";
                Pass = " ";
                HidePass = " ";
                //Register
                /*if (Rcheck == 0)
                {*/
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
                /*}
                else if(Rcheck == 1)
                {*/
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
                /*}
                else
                {*/
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
                //}
            }
            
            
        }
        Render();
    }
    //Disable text input
    SDL_StopTextInput();
    cout << Name << " " << Pass << endl;
    //cout << RName << " " << RPass << endl;
    return true;
}

void Login::Render()
{
    //SDL_Color textColor = { 0, 0, 0, 0xFF };
    SDL_RenderClear(gRenderer);
    if (Log)
    {
        //cout << "t: " << t << endl;
        if (t <= 99 && t >= 0)
        {
            if (Lcheck)
            {
                Name.pop_back();
                Name += "|";
            }
            else
            {
                Pass.pop_back();
                HidePass.pop_back();
                Pass += "|";
                HidePass += "|";
            }
            
        }
        else if(t <= 199)
        {
            if (Lcheck)
            {
                Name.pop_back();
                Name += " ";
            }
            else
            {
                Pass.pop_back();
                HidePass.pop_back();
                Pass += " ";
                HidePass += " ";
            }
            
        }
        t++;
        if (t >= 200) t = 0;
        Llogin.render(0, 0);
        LNinputTextTexture.render(450, 273);
        LPinputTextTexture.render(450, 378);
    }

    else
    {
        if (T <= 99 && T >= 0)
        {
            if (Rcheck == 0)
            {
                RName.pop_back();
                RName += "|";
            }
            else if(Rcheck == 1)
            {
                RPass.pop_back();
                RHidePass.pop_back();
                RPass += "|";
                RHidePass += "|";
            }
            else
            {
                RCPass.pop_back();
                RCHidePass.pop_back();
                RCPass += "|";
                RCHidePass += "|";
            }

        }
        else if (T <= 199)
        {
            if (Rcheck == 0)
            {
                RName.pop_back();
                RName += " ";
            }
            else if(Rcheck == 1)
            {
                RPass.pop_back();
                RHidePass.pop_back();
                RPass += " ";
                RHidePass += " ";
            }
            else
            {
                RCPass.pop_back();
                RCHidePass.pop_back();
                RCPass += " ";
                RCHidePass += " ";
            }

        }
        T++;
        if (T >= 200) T = 0;
        Lregister.render(0, 0);
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

bool Login::getGuest()
{
    return checkGuest;
}
