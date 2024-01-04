#pragma once
#include "CommonFunc.h"
#include "LTexture.h"

using namespace std;

class Winner {
	LTexture WTexture;
	LTexture RankTexture;
	LTexture TimeTexture;
	LTexture StepTexture;
	int step;
	int rank;
	string time;
	SDL_Rect RankRect[11];
	bool Quit;
	SDL_Event e;
	bool isPressBack;
	bool isPressReload;
public:
	Winner(int step, string time, int rank) : step(step), time(time), rank(rank)
	{
		Quit = false;
		isPressBack = false;
		isPressReload = false;
		for (int i = 0; i < 11; ++i)
		{
			RankRect[i] = { 0, 70 * i, 184, 72 };
		}
	}
	
	bool GetIsPressBack() { return isPressBack; }
	bool GetIsPressReload() { return isPressReload; }

	bool loadMedia()
	{
		bool success = true;
		if (!WTexture.loadFromFile("Data//Winner//WTexture.png", false))
		{
			success = false;
			cout << "Can't Load the WTexture SDL_Error: " << SDL_GetError() << " IMG Error: " << IMG_GetError() << endl;
		}
		else if (!RankTexture.loadFromFile("Data//Winner//RankRect.png", false))
		{
			success = false;
			cout << "Can't Load the RankTexture SDL_Error: " << SDL_GetError() << " IMG Error: " << IMG_GetError() << endl;
		}
		else if (!StepTexture.loadFromRenderedText(to_string(step), {0x00, 0x00, 0x00, 0xFF}, 34))
		{
			success = false;
			cout << "Can't Load the RankTexture SDL_Error: " << SDL_GetError() << " IMG Error: " << IMG_GetError() << endl;
		}
		else if (!TimeTexture.loadFromRenderedText(time, { 0x00, 0x00, 0x00, 0xFF }, 34))
		{
			success = false;
			cout << "Can't Load the RankTexture SDL_Error: " << SDL_GetError() << " IMG Error: " << IMG_GetError() << endl;
		}
		return success;
	}

	~Winner()
	{
		time.clear();
		WTexture.free();
		RankTexture.free();
	}

	void HandleEvent()
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				Quit = true;
				outGame = true;
				break;
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
			{
				Quit = true;
				outGame = true;
			}
			else
			{
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					cout << e.motion.x << " " << e.motion.y << endl;
					//Nut Ok va xem trang thai Hoan Thanh
					if (e.motion.x >= 588 && e.motion.x <= 689 && e.motion.y >= 513 && e.motion.y <= 577)
					{
						Quit = true;
					}
					//Nut choi lai
					if (e.motion.x >= 702 && e.motion.x <= 767 && e.motion.y >= 515 && e.motion.y <= 580)
					{
						Quit = true;
						isPressReload = true;
					}
					//Quay ve Menu  MenuStart
					if (e.motion.x >= 509 && e.motion.x <= 574 && e.motion.y >= 513 && e.motion.y <= 578)
					{
						Quit = true;
						isPressBack = true;
					}
				}
			}
		}
	}

	void render()
	{
		WTexture.render(399, 79);
		RankTexture.render(553, 425, &RankRect[rank]);
		TimeTexture.render((SCREEN_WIDTH - TimeTexture.getWidth()) / 2, 260);
		StepTexture.render((SCREEN_WIDTH - StepTexture.getWidth()) / 2, 358);
		SDL_RenderPresent(gRenderer);
	}

	void run()
	{
		if (!loadMedia())
		{
			cout << "Can't Load Media Winner " << endl;
			return;
		}
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

		// Đặt màu vẽ là đen (0, 0, 0, 200) - mờ
		SDL_SetRenderDrawColor(gRenderer, 0xDC, 0xBA, 0x79, 255 * 23 / 100);

		// Tạo một SDL_Rect có kích thước như bạn muốn
		SDL_Rect FillRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		// Vẽ hình chữ nhật mờ lên Renderer
		SDL_RenderFillRect(gRenderer, &FillRect);
		// Đặt lại chế độ blend về mặc định
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		while (!Quit)
		{
			HandleEvent();
			render();
		}
	}
};