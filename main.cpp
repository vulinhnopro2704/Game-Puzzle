#include "Main_Menu.h"
#include "Winner.h"
#include "LeaderBoard.h"
#include "Setting.h"

using namespace std;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

bool isUSE = true;
int n = 0;
int Order = 0;
int Mode = -1;
bool isInit = false;
bool outGame = false;
bool ShowNumber = true;
bool SoundOn = true;
int Volume = 126;
int TOTAL_IMAGE = 4;

int main(int argc, char* argv[])
{
    isUSE = false;
    MainMenu a;
    a.run();
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    if (gFont != NULL) {
        TTF_CloseFont(gFont);
        gFont = NULL;
    }

    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
	return 0;
}

//int main(int argc, char* argv[])
//{
//	MainMenu b;
//	b.init();
//	Setting a;
//	a.Run();
//	return 0;
//}

