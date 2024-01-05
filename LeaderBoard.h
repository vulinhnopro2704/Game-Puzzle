#pragma once
#ifndef LEADER_BOARD_H_
#define LEADER_BOARD_H_

#include "CommonFunc.h"

using namespace std;

struct Record {
	std::string name;
	std::string time;
	int step;
	Record() {}
	Record(std::string name_i, std::string time_i, int step_i)
	{
		name = name_i;
		time = time_i;
		step = step_i;
	}
};

enum ModeLeaderBoard {
	Mode3x3 = 0,
	Mode4x4,
	Mode5x5,
	Mode6x6,
	ModeTotal = 4
};

class LeaderBoard {
private:
	std::vector<std::vector<Record>> LeaderBoardRecord;
	LTexture LBTexture;
	SDL_Renderer* LBRenderer;
	//Mốc rank hiện tại
	int Current;
	//Chế độ hiện tại
	int CurrentMode;
	int MAX = 20;

	const int ButtonWidth = 134;
	const int ButtonHeight = 71;
	const int y = 81;

	std::vector<std::vector<LTexture>> StepTexture;
	std::vector<std::vector<LTexture>> NameTexture;
	std::vector<std::vector<LTexture>> TimeTexture;
	std::vector<std::vector<LTexture>> RankTexture;
	SDL_Rect *SpriteRect;
	std::vector<LTexture> ModeDisplay;
	std::vector<LButton> ModeDisplayButton;
	LButton Back;
	bool Quit;
	SDL_Event e;
	SDL_Color TextColor = { 0, 0, 0, 0xFF };
public:
	LeaderBoard() : SpriteRect(new SDL_Rect[3]) {
		Current = 0;
		Quit = false;
		SpriteRect[0] = { 0, 0, 154, 71 };
		SpriteRect[1] = { 0, 99, 154, 71 };
		SpriteRect[2] = { 0, 198, 154, 71 };
		// Initialize vectors with proper size
		StepTexture.resize(ModeTotal, std::vector<LTexture>(MAX));
		NameTexture.resize(ModeTotal, std::vector<LTexture>(MAX));
		TimeTexture.resize(ModeTotal, std::vector<LTexture>(MAX));
		RankTexture.resize(ModeTotal, std::vector<LTexture>(MAX));
		ModeDisplay.resize(ModeTotal);
		ModeDisplayButton.resize(ModeTotal);
		LeaderBoardRecord.resize(ModeTotal);
		Back.SetAllValue(1159, 8, 101, 101);
		ModeDisplayButton[0].SetAllValue(142, 81, ButtonWidth, ButtonHeight);
		ModeDisplayButton[1].SetAllValue(323, 81, ButtonWidth, ButtonHeight);
		ModeDisplayButton[2].SetAllValue(817, 81, ButtonWidth, ButtonHeight);
		ModeDisplayButton[3].SetAllValue(997, 81, ButtonWidth, ButtonHeight);

		// ... rest of your constructor code ...
	}
	~LeaderBoard() {
		LeaderBoardRecord.clear();
		LBTexture.free();
	}

	void RemoveAllScore()
	{
		LeaderBoardRecord.clear();
	}

	void DisplayScore()
	{
		for (int i = 0; i < LeaderBoardRecord.size(); ++i)
		{
			for (int j = 0; j < LeaderBoardRecord[i].size(); ++j)
			{
				cout << LeaderBoardRecord[i][j].name << " " << LeaderBoardRecord[i][j].time << " " << LeaderBoardRecord[i][j].step << endl;
			}
		}
	}

	//Trả về true nếu R1 xếp hạng cao hơn R2 (Thời gian bé hơn (hoặc số bước bé hơn nếu thời gian bằng)
	static bool Compare(Record R1, Record R2)
	{
		int check = strcmp(R1.time.c_str(), R2.time.c_str());
		if (check < 0)
		{
			return true;
		}
		else if (check == 0)
		{
			return (R1.step < R2.step) ? true : false;
		}
		return false;
	}

	void Add(struct Record R, ModeLeaderBoard mode) {
		LeaderBoardRecord[static_cast<int>(mode)].push_back(R);
		sort(LeaderBoardRecord[static_cast<int>(mode)].begin(), LeaderBoardRecord[static_cast<int>(mode)].end(), Compare);
		if (LeaderBoardRecord[static_cast<int>(mode)].size() > 10) LeaderBoardRecord[static_cast<int>(mode)].pop_back();
	}

	bool SaveToFile(ModeLeaderBoard mode) {
		const string filename = "LeaderBoard-Record//HighScore.txt";
		std::ofstream file(filename);

		if (file.is_open()) {
			for (const auto& record : LeaderBoardRecord[static_cast<int>(mode)]) {
				file << record.name << "," << record.time << "," << record.step << std::endl;
			}
			file.close();
			std::cout << "Data has been saved to file " << filename << std::endl;
			return true;
		}
		else {
			std::cerr << "Unable to open file to save data." << std::endl;
			return false;
		}
	}

	bool LoadFromFile(ModeLeaderBoard mode) {
		bool success = true;
		if (!LBTexture.loadFromFile("IMG//LeaderBoard//Leader Board - Main.png", false))
		{
			cout << "Can't load the LBTexture!! SDL ERROR: " << SDL_GetError() << " IMG ERROR: " << IMG_GetError() << endl;
			success = false;
		}

		for (int i = 0; i < ModeTotal; ++i)
		{
			ModeDisplay[i].loadFromFile("IMG//LeaderBoard//Mode" + to_string(i) + ".png", false);
		}

		const std::string filename = "LeaderBoard-Record//HighScore" + to_string(mode + 3) + ".txt";

		std::ifstream file(filename);
		LeaderBoardRecord[static_cast<int>(mode)].clear(); // Clear old data from the vector before reading from the file

		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				std::istringstream ss(line);
				std::string name, time;
				int step;

				std::getline(ss, name, ',');
				std::getline(ss, time, ',');
				ss >> step;

				LeaderBoardRecord[static_cast<int>(mode)].emplace_back(name, time, step);
			}
			file.close();
			std::cout << "Data has been successfully read into LeaderBoardScore vector " << filename << std::endl;
			for (int i = 0; i < LeaderBoardRecord[static_cast<int>(mode)].size(); ++i) {
				cout << LeaderBoardRecord[static_cast<int>(mode)][i].name << " "
					<< LeaderBoardRecord[static_cast<int>(mode)][i].time << " "
					<< LeaderBoardRecord[static_cast<int>(mode)][i].step << endl;
				sort(LeaderBoardRecord[static_cast<int>(mode)].begin(),
					LeaderBoardRecord[static_cast<int>(mode)].end(), Compare);
			}
		}
		else {
			std::cerr << "Unable to open file to read data." << std::endl;
			success = false;
		}
		return success;
	}

	void LoadAllData()
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < LeaderBoardRecord[i].size(); ++j)
			{
				std::string tmp = to_string(j + 1);
				RankTexture[i][j].loadFromRenderedText(tmp, TextColor, 36);
				StepTexture[i][j].loadFromRenderedText(to_string(LeaderBoardRecord[i][j].step), TextColor, 36);
				NameTexture[i][j].loadFromRenderedText(LeaderBoardRecord[i][j].name, TextColor, 36);
				TimeTexture[i][j].loadFromRenderedText(LeaderBoardRecord[i][j].time, TextColor, 36);
			}
		}
	}

	void render(ModeLeaderBoard mode)
	{
		LBTexture.render(0, 0);
		for (int i = 0; i < ModeTotal; ++i)
		{
			ModeDisplayButton[i].render(ModeDisplay[i], SpriteRect);
		}
		ModeDisplayButton[CurrentMode].render(ModeDisplay[CurrentMode], SpriteRect, true);
		for (int i = 0; i < 5; ++i)
		{
			RankTexture[mode][Current + i].render(186, 272 + 78 * i);
			StepTexture[mode][Current + i].render(989, 272 + 78 * i);
			NameTexture[mode][Current + i].render(325, 272 + 78 * i);
			TimeTexture[mode][Current + i].render(782, 272 + 78 * i);
		}
	}
	void HandleEvent()
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				Quit = true;
				outGame = true;
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
			{
				Quit = true;
				outGame = true;
			}
			if (e.wheel.y < 0)
			{
				if (Current + 1 < MAX - 4)
					++Current;
			}
			else if (e.wheel.y > 0)
			{
				if (Current - 1 >= 0)
					--Current;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				Back.HandleEvent(&e);
				if (Back.getCurrentSprite() == BUTTON_SPRITE_MOUSE_DOWN)
				{
					Quit = true;
				}
				for (int i = 0; i < 4; ++i)
				{
					ModeDisplayButton[i].HandleEvent(&e);
					if (ModeDisplayButton[i].getCurrentSprite() == BUTTON_SPRITE_MOUSE_DOWN)
					{
						CurrentMode = i;
					}
				}

			}
			
		}
	}

	int ReturnRank(int mode, Record R)
	{
		if (!LoadFromFile(ModeLeaderBoard(mode)))
		{
			cout << "LeaderBoard can't load some file " << endl;
			return -1;
		}
		for (int i = 0; i < 10; ++i)
		{
			if (!Compare(LeaderBoardRecord[mode][i], R))
				return i;
		}
		return 10; //unrank;
	}

	void run()
	{
		for (int i = Mode3x3; i < ModeTotal; ++i)
		{
			if (!LoadFromFile(ModeLeaderBoard(i)))
			{
				cout << "LeaderBoard can't load some file " << endl;
				return;
			}
		}
		LoadAllData();
		DisplayScore();
		while (!Quit)
		{
			LBTexture.render(0, 0);
			HandleEvent();
			render(ModeLeaderBoard(CurrentMode));
			SDL_RenderPresent(gRenderer);
		}
	}
};

#endif // LEADER_BOARD_H_