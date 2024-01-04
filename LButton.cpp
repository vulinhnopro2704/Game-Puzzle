#include "LButton.h"

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}


void LButton::SetPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::SetAllValue(int x, int y, int BUTTOM_WIDTH, int BUTTOM_HEIGHT) 
{
	mPosition.x = x;
	mPosition.y = y;
	ButtonWidth = BUTTOM_WIDTH;
	ButtonHeight = BUTTON_HEIGHT;
}

void LButton::HandleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);
		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + ButtonWidth)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + ButtonHeight)
		{
			inside = false;
		}
		//Mouse is outside button
		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

			}
		}
	}
}

void LButton::render(LTexture& gButtonSpriteSheetTexture, SDL_Rect gSpriteClips[], bool isUsed)
{
	if (isUsed)
		gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[BUTTON_SPRITE_MOUSE_DOWN]);
	else
		//Show current button sprite
		gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}

LButtonSprite LButton::getCurrentSprite()
{
	return mCurrentSprite;
}
