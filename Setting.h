#pragma once

#include "CommonFunc.h"

class Setting
{
private:
	LTexture LSetting;
	LTexture LVolume;
	LTexture LImage;
	LTexture LSound;
	LTexture LShowNumber;
	LTexture LSoundTrackNumber;
	LTexture LLoadNewImage;
	SDL_Rect SoundRect[2];
	SDL_Rect ShowNumberRect[2];
	int OrderImage;
	int OrderSoundTrack;
	bool Quit = false;
	SDL_Event e;
public:
	Setting() {
		SoundRect[0] = { 0, 0, 43, 41 };
		SoundRect[1] = { 0, 41, 43, 41 };
		ShowNumberRect[0] = { 0, 0, 93, 50 };
		ShowNumberRect[1] = { 0, 55, 93, 50 };
		const wchar_t* folderPath = L"Data//MenuImage";
		TOTAL_IMAGE = countPNGFiles(folderPath);
	}
	int countPNGFiles(const wchar_t* folderPath);
	bool LoadFromFile();
	void Render();
	void HandleEvent();
	void Run();
	~Setting() {
		LSetting.free();
		LVolume.free();
		LImage.free();
		LSound.free();
		LShowNumber.free();
	}
};