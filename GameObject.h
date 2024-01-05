#pragma once
#include "Gameplay.h"
#include "TextureManager.h"

class GameObject {

public:
	GameObject(const char* texturesheet, SDL_Renderer* ren, int x, int y);
	GameObject(SDL_Texture* Texture, SDL_Renderer* ren, int x, int y);

	void Update(int x, int y, int Width, int Height);
	~GameObject();

	void Update(int x, int y, int X, int Y, int Width, int Height);
	void Render();


private:

	int xpos;
	int ypos;
	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* renderer;

};