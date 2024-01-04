#include "Help.h"

Help::Help()
{
	isRunning = true;
	isBackHelp = false;
}

void Help::run()
{
	if (!HTexture.loadFromFile("IMG//Help_Game.PNG")) {
		std::cout << "Can't not load Help.png : " << IMG_GetError();
	}
	else {
		SDL_Event e;
		while (Help::isRunning)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					Help::isRunning = false;
					outGame = true;
				}
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					if (e.motion.y >= 600)
					{
						Help::isRunning = false;
						isBackHelp = true;
						outGame = false;
					}
				}
			}
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);
			HTexture.Resize(SCREEN_WIDTH, SCREEN_HEIGHT);
			HTexture.render(0, 0);
			SDL_RenderPresent(gRenderer);
		}
	}
}

Help::~Help()
{
	HTexture.free();
}