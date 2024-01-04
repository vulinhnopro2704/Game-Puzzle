#pragma once
#ifndef  LBUTTON_H_
#define LBUTTON_H_
#include "LTexture.h" 

class LButton {
public:
	//Initialize internal variable
	LButton();

	//Sets Top Left Position
	void SetPosition(int x, int y);
	void SetAllValue(int x, int y, int BUTTOM_WIDTH, int BUTTOM_HEIGHT);
	//Handle Event 
	void HandleEvent(SDL_Event* e);

	//Shows button sprites
	void render(LTexture& gButtonSpriteSheetTexture, SDL_Rect gSpriteClips[], bool isUsed = FALSE);

	LButtonSprite getCurrentSprite();
private:
	// Top left Position of Button
	SDL_Point mPosition;

	//Size of Button
	int ButtonWidth;
	int ButtonHeight;

	//Currently Used global sprite
	LButtonSprite mCurrentSprite;
};

#endif // ! LBUTTON_H_
