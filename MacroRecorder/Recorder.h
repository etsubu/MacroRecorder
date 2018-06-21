#pragma once
#include "MacroCompiler.h"
#include <vector>

#define TYPE_MOUSE 0
#define TYPE_KEYBOARD 1
#define TYPE_SLEEP 2

class Recorder {
public:
	Recorder(MacroCompiler *compiler);
	~Recorder();
	void record();
	void keyboardEvent(unsigned int action, DWORD key);
	void mouseEvent(unsigned char action);

private:
	void addAction(unsigned int action, unsigned char type, DWORD key);
	bool wait;
	bool active;
	DWORD lastAction;
	std::vector<Action*> actionList;
	MacroCompiler *compiler;
};