#pragma once

#include "CommonFunc.h"

class Setting
{
private:
	LTexture LSetting;
	LTexture LVolume;
	LTexture LSound;
	LTexture LShowNumber;
	LTexture LSoundTrackNumber;
	LTexture LLoadNewImage;
	LTexture DragAndDrop;
	LTexture Drag;
	SDL_Rect SoundRect[2];
	SDL_Rect ShowNumberRect[2];
	int OrderImage;
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
	void DropScreen(string s);
	void DropScreen();
	void DragScreen();
	bool LoadFromFile();
	void Render();
	void HandleEvent();
	void Run();
	~Setting();
};