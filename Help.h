#pragma once
#ifndef  HELP_H
#define HELP_H

#include "LTexture.h"

class Help {
	LTexture HTexture;
	bool isRunning;
	bool isBackHelp;
public:
	Help();
	void run();
	~Help();
	bool getIsBackHelp() { return isBackHelp;  }
};

#endif // ! HELP_H
