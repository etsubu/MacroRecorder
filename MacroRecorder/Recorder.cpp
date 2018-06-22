#include "stdafx.h"

Recorder::Recorder(MacroCompiler *compiler) {
	wait = true;
	active = false;
	this->compiler = compiler;
	this->lastAction = 0;
}

Recorder::~Recorder() {
	for (auto act : actionList)
		delete act;
}

void Recorder::record() {
	std::cout << "Press F1 to start recording and F1 again to stop recording..." << std::endl;
	registerRecorder(this);

	while (wait)
		Sleep(1);
	active = true;
	wait = true;

	while (wait)
		Sleep(1);

	active = false;
	std::string filename;
	std::cout << "Enter name for the macro (empty for discard): "<< std::endl;
	std::getline(std::cin, filename);
	if (!filename.empty()) {
		std::vector<std::string> macroAsText;
		if (!compiler->decompileMacro(this->actionList, macroAsText)) {
			std::cout << "There was an error while decompiling macro!" << std::endl;
			return;
		}
		std::ofstream os;
		os.open(filename, std::ofstream::binary);
		if (os.is_open()) {
			for (std::string line:macroAsText) {
				os << line << "\n";
			}
		}
		os.close();
	}
}

void Recorder::addAction(unsigned int action, unsigned char type, DWORD key) {
	DWORD ticks = GetTickCount();
	if (!actionList.empty()) {
		// Push sleep action
		Action *act = new Action;
		act->action = 0;
		act->type = TYPE_SLEEP;
		act->key = ticks - lastAction;
		act->x = 0;
		act->y = 0;
		actionList.push_back(act);
	}
	POINT loc;
	if (GetCursorPos(&loc)) {
		Action *act = new Action;
		act->action = action;
		act->type = type;
		act->key = key;
		act->x = loc.x;
		act->y = loc.y;
		actionList.push_back(act);
	}
	lastAction = ticks;
}

void Recorder::mouseEvent(unsigned char action) {
	if (!active)
		return;
	addAction(action, TYPE_MOUSE, 0x00);
}

void Recorder::keyboardEvent(unsigned int action, DWORD key) {
	if (key == VK_F1) {
		if(action + 0x100 == WM_KEYUP)
			wait = !wait;
	}
	if (!active)
		return;
	addAction(action, TYPE_KEYBOARD, key);
}