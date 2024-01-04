#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(SDL_Texture* Texture, SDL_Renderer* ren, int x, int y)
{
	objTexture = Texture;
	renderer = ren;
	xpos = x;
	ypos = y;
}

void GameObject::Update(int x, int y, int Width, int Height)
{
	xpos = x;
	ypos = y;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = Width;
	destRect.h = Height;
}

void GameObject::Update(int x, int y, int X, int Y, int Width, int Height)
{
	xpos = x;
	ypos = y;

	srcRect.h = X;
	srcRect.w = Y;
	//srcRect.h = 512;
	//srcRect.w = 512;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = Width;
	destRect.h = Height;

}



void GameObject::Render()
{
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}