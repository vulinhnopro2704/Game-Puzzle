#include "Setting.h"

Mix_Music* backgroundMusic;

int Setting::countPNGFiles(const wchar_t* folderPath) {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    int pngFileCount = 0;
    std::wstring searchPath = std::wstring(folderPath) + L"\\*.png";

    hFind = FindFirstFileW(searchPath.c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                pngFileCount++;
            }
        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
    else {
        std::wcerr << L"Không thể mở thư mục hoặc không có file .png!" << std::endl;
    }

    return pngFileCount;
}

void Setting::DropScreen(string s)
{
    LTexture tmp;
    tmp.loadFromRenderedText(s, { 0,0,0 }, 40);
    DragAndDrop.render(0, 0);
    tmp.render((SCREEN_WIDTH - tmp.getWidth()) / 2, SCREEN_HEIGHT / 2 - 20);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(1500);
}
bool isDrag = false;

//void Setting::DragScreen()
//{
//    isDrag = true;
//    Drag.render(0, 0);
//    SDL_RenderPresent(gRenderer);
//}

bool Setting::LoadFromFile()
{
    if (!LSetting.loadFromFile("Data//Setting//Setting.png"))
    {
        return false;
    }
    else if (!LShowNumber.loadFromFile("Data//Setting//ShowNumber.png", false))
    {
        return false;
    }
    else if (!LSound.loadFromFile("Data//Setting//Sound.png", false))
    {
        return false;
    }
    else if (!DragAndDrop.loadFromFile("Data//Loading//DragAndDrop.png"), false)
    {
        return false;
    }
    else if (!Drag.loadFromFile("Data//Loading//Drag.png"))
    {
        return false;
    }
    else if (!LVolume.loadFromFile("Data//Setting//Volume.png"))
    {
        cout << "Loi here" << endl;
        return false;
    }
    return true;
}

void Setting::Render()
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    int tmpx = 1.0 * 343 - (1.0 * 128 - 1.0 * Volume) / 128 * 425 * 1.0;
    cout << Volume << "  " << tmpx << endl;
    LVolume.render(tmpx, 271);
    //cout << 343 - (1 - Volume / 128) * 425 << endl;
    LSetting.render(0, 0);
    LShowNumber.render(951, 194, &ShowNumberRect[ShowNumber]);
    LSound.render(827, 283, &SoundRect[SoundOn]);
    LSoundTrackNumber.loadFromRenderedText("Sound " + to_string(OrderSoundtrack), { 0xC5, 0x5A, 0x11, 0xFF }, 30);
    LSoundTrackNumber.render(372, 196);
    SDL_RenderPresent(gRenderer);
}

void Setting::HandleEvent()
{
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            Quit = true;
            outGame = true;
        }
        if (e.type == SDL_DROPFILE) {
            // Đọc file ảnh từ đường dẫn được kéo thả
            SDL_Surface* loadedSurface = IMG_Load(e.drop.file);
            SDL_free(e.drop.file);
            if (loadedSurface == nullptr) {
                DropScreen("Khong the tai anh!");
                return;
            }
            // Lưu file ảnh dưới định dạng PNG
            std::string newImagePath = "Data//MenuImage//MenuImage" + to_string(TOTAL_IMAGE) + ".png";
            TOTAL_IMAGE++;
            if (IMG_SavePNG(loadedSurface, newImagePath.c_str()) != 0) {
                DropScreen("Loi khi luu file!");
            }
            DropScreen("Luu  anh thanh cong");
             SDL_FreeSurface(loadedSurface);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            //Thoát Cài đặt
            if (e.motion.x > 1146 && e.motion.x < 1250 && e.motion.y > 19 && e.motion.y < 130)
            {
                Quit = true;
            }
            //Nhạc nền và ảnh
            else if (e.motion.y > 192 && e.motion.y < 225)
            {
                //Đổi nhạc
                if (e.motion.x > 328 && e.motion.x < 580)
                {
                    if (backgroundMusic != NULL) {
                        Mix_FreeMusic(backgroundMusic);
                        backgroundMusic = NULL;
                    }
                    if (e.motion.x > 543)
                    {
                        OrderSoundtrack = (OrderSoundtrack + 1) % TOTAL_SOUNDTRACK;
                        string tmp = "Music//Soundtrack" + to_string(OrderSoundtrack) + ".mp3";
                    }
                    else if (e.motion.x < 360)
                    {
                        OrderSoundtrack = (OrderSoundtrack - 1);
                        if (OrderSoundtrack < 0) OrderSoundtrack = TOTAL_SOUNDTRACK - 1;
                    }
                    string tmp = "Music//Soundtrack" + to_string(OrderSoundtrack) + ".mp3";
                    Mix_HaltMusic();

                    backgroundMusic = Mix_LoadMUS(tmp.c_str());
                    if (backgroundMusic == NULL) {
                        printf("Failed to load music! SDL_Mixer Error: %s\n", Mix_GetError());
                    }
                    Mix_PlayMusic(backgroundMusic, -1);
                }
                //Bật/tắt Hiện số trên ảnh
                else if (e.motion.x > 931 && e.motion.x < 1040)
                {
                    ShowNumber = (!ShowNumber);
                }
            }
            // Âm lượng
            else if (e.motion.y > 271 && e.motion.y < 320)
            {
                //Bật/tắt âm lượng
                if (e.motion.x > 828 && e.motion.x < 875)
                {
                    SoundOn = (!SoundOn);
                    if (!SoundOn) Volume = 0;
                    else Volume = 128;
                    // Đặt âm lượng cho Mix_Chunk (hiệu ứng âm thanh)
                    Mix_Volume(-1, Volume); // Đặt âm lượng cho tất cả các kênh hiệu ứng âm thanh

                    // Đặt âm lượng cho nhạc nền (Mix_Music)
                    Mix_VolumeMusic(Volume); // Đặt âm lượng cho nhạc nền
                }
                //Tùy chỉnh âm lượng
                else if (e.motion.x > 342 && e.motion.x < 769)
                {
                    Volume = min(127, (1.0 * e.motion.x - 343) / 425 * 128);
                    if (Volume == 0) SoundOn = false;
                    else SoundOn = true;
                    // Đặt âm lượng cho Mix_Chunk (hiệu ứng âm thanh)
                    Mix_Volume(-1, Volume); // Đặt âm lượng cho tất cả các kênh hiệu ứng âm thanh

                    // Đặt âm lượng cho nhạc nền (Mix_Music)
                    Mix_VolumeMusic(Volume); // Đặt âm lượng cho nhạc nền
                }
            }
        }
    }  
}

void Setting::Run()
{
    if (!LoadFromFile())
    {
        cout << "Can't Open Setting!!!" << endl;
        return;
    }
    while (!Quit)
    {
        HandleEvent();
        Render();
    }
}

Setting::~Setting() {
    LSetting.free();
    LVolume.free();
    LSound.free();
    LShowNumber.free();
    LSoundTrackNumber.free();
    LLoadNewImage.free();
    DragAndDrop.free();
    Drag.free();

    if (backgroundMusic != nullptr) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }
}