#include "GetPlayerName.h"

GetPlayerName::GetPlayerName()
{
    inputText = "";
    isHadNameInput = false;
}

bool GetPlayerName::HandleEvent()
{
    //cout << "Handle Event " << endl;
    //Main loop flag
    bool quit = false;

    //Set text color as black
    SDL_Color textColor = { 0, 0, 0, 0xFF };

    //The current input text.
    gInputTextTexture.loadFromRenderedText(inputText.c_str(), textColor);
    //Enable text input
    SDL_StartTextInput();
    //While application is running
    while (!quit)
    {
        //The rerender text flag
        bool renderText = false;

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                outGame = true;
                break;
            }
            //Special key input
            else if (e.type == SDL_KEYDOWN)
            {
                //Handle backspace
                if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
                {
                    //lop off character
                    inputText.pop_back();
                    renderText = true;
                }
                //Handle copy
                else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
                {
                    SDL_SetClipboardText(inputText.c_str());
                }
                //Handle paste
                else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
                {
                    inputText = SDL_GetClipboardText();
                    renderText = true;
                }
                else if ((e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_RETURN2) && inputText.length() > 0)
                {
                    quit = true;
                    isHadNameInput = true;
                }
            }
            //Special text input event
            else if (e.type == SDL_TEXTINPUT)
            {
                //Not copy or pasting
                if (inputText.size() <= 16 && !(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                {
                    //Append character
                    inputText += e.text.text;
                    renderText = true;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = e.motion.x, y = e.motion.y;
                if (x >= 977 && x <= 977 + 115 && y >= 111 && y <= 226)
                {
                    quit = true;
                    isHadNameInput = false;
                    break;
                }
                else if (x >= 407 && x <= 407 + 470 && y >= 509 && y <= 613 && inputText.length() > 0)
                {
                    quit = true;
                    isHadNameInput = true;
                }
            }
        }
        //Rerender text if needed
        if (renderText)
        {
            //Text is not empty
            if (inputText != "")
            {
                //Render new text
                gInputTextTexture.loadFromRenderedText(inputText.c_str(), textColor, 35);
            }
            //Text is empty
            else
            {
                //Render space texture
                gInputTextTexture.loadFromRenderedText(" ", textColor);
            }
        }
        render();
    }
    //Disable text input
    SDL_StopTextInput();
    return ((inputText != "") ? true : false) && isHadNameInput;
}

bool GetPlayerName::LoadMedia()
{
    //cout << "Load Media" << endl;
    bool success = true;
    if (!GPNTexture.loadFromFile("IMG//GetPlayerInputName//PlayerNameInput.png"))
    {
        cout << "Can't load the GPNTexture!! SDL ERROR: " << SDL_GetError() << " IMG ERROR: " << IMG_GetError() << endl;
        success = false;
    }
    return success;
}

void GetPlayerName::render()
{
    //cout << "Rendered" << endl;
    //Clear screen
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    // Đặt màu vẽ là đen (0, 0, 0, 200) - mờ
    SDL_SetRenderDrawColor(gRenderer, 0xDC, 0xBA, 0x79, 23 / 100 * 0xFF);

    // Tạo một SDL_Rect có kích thước như bạn muốn
    SDL_Rect FillRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    // Vẽ hình chữ nhật mờ lên Renderer
    SDL_RenderFillRect(gRenderer, &FillRect);
    // Đặt lại chế độ blend về mặc định
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
    //Render the Game Play Name Texture to center of the Screen

    GPNTexture.render((SCREEN_WIDTH - GPNTexture.getWidth()) / 2, (SCREEN_HEIGHT - GPNTexture.getHeight()) / 2);

    //Render text textures
    gInputTextTexture.render((SCREEN_WIDTH - gInputTextTexture.getWidth()) / 2, 395);

    //Update screen
    SDL_RenderPresent(gRenderer);
    //Update screen
    SDL_RenderPresent(gRenderer);
}

bool GetPlayerName::run()
{
    if (!LoadMedia()) return false;
    if (!HandleEvent()) return false;
    return true;
}

string GetPlayerName::getInputText()
{
    return inputText;
}

GetPlayerName::~GetPlayerName()
{
    gInputTextTexture.free();
    GPNTexture.free();
}