#pragma once

#include "stdafx.h"

class Emulator {
public:
	Emulator();
	~Emulator();
	int playMacro(const std::vector<Action*> &actionList);
private:
	int doMouseAction(Action *action);
	int doKeyboardAction(Action *action);
	std::map<std::string, unsigned char> textToKeyboardAction;
	std::map<std::string, unsigned char> textToMouseAction;
};