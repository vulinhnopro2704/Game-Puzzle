#pragma once
#include"CommonFunc.h"
class Login
{
	// Check account
	map<string, bool> CheckA;
	map<string, pair<string, int>> Account;
	LTexture Llogin;
	LTexture Lregister;
	//login name
	LTexture LNinputTextTexture;
	//register name
	LTexture RNinputTextTexture;
	//login pass
	LTexture LPinputTextTexture;
	//register pass
	LTexture RPinputTextTexture;
	//register confirm pass
	LTexture RCPinputTextTexture;
	LTexture LAnnounce;
	LTexture Alert;
	LTexture LNTrochuot;
	LTexture LPTrochuot;
	bool Lquit = false;
	bool Lcheck = true;
	bool Hide = true;
	bool Log = true;
	int Rcheck = 0;
	int CountAccount;
	string Name = "|";
	string Pass = " ";
	string HidePass = " ";
	string RHidePass = " ";
	string RCHidePass = " ";
	string RName = "|";
	string RPass = " ";
	string RCPass = " ";
	bool Trocheck = false;
	bool checkGuest = false;
	int t = 201;
	int T = 201;
public:
	Login();
	~Login();
	void RenderAnnounce(string s);
	bool Register();
	bool login();
	bool LoadFromFile();
	bool HandleEvent();
	void Render();
	void Run();
	bool Init();
	string getName();
	void TaoFile(string num, string type);
	int getNumFile();
	bool getGuest();
};

