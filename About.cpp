#include "About.h"

About::About()
{
	isRunning = true;
	if (!HTexture.loadFromFile("Data//About//About.png", false)) {
		std::cout << "Can't not load About.png : " << IMG_GetError();
	}
}

void About::run()
{
	SDL_Event e;
	while (isRunning)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				isRunning = false;
				outGame = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.motion.y <= 77 && e.motion.y >= 27 && e.motion.x >= 1200)
				{
					isRunning = false;
				}
			}
		}
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		HTexture.render(0, 0);
		SDL_RenderPresent(gRenderer);
	}
}

About::~About()
{
	HTexture.free();
}
