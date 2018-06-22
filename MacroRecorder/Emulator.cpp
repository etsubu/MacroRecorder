#include "stdafx.h"

Emulator::Emulator() {

}

Emulator::~Emulator() {

}

int Emulator::doMouseAction(Action *action) {
	if (!SetCursorPos(action->x, action->y))
		return 0;
	INPUT input[1] = { 0 };

	/*input[0].type = INPUT_MOUSE;
	input[0].mi.dx = action->x; // desired X coordinate
	input[0].mi.dy = action->y; // desired Y coordinate
	input[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	*/
	input[0].type = INPUT_MOUSE;
	std::cout << (DWORD)action->action << std::endl;
	input[0].mi.dwFlags = action->action;

	SendInput(1, input, sizeof(INPUT));
	return 1;
}

int Emulator::doKeyboardAction(Action *action) {
	INPUT input[2] = {0};
	input[0].type = INPUT_MOUSE;
	input[0].mi.dx = action->x; // desired X coordinate
	input[0].mi.dy = action->y; // desired Y coordinate
	input[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wScan = MapVirtualKey(action->key, MAPVK_VK_TO_VSC);
	input[1].ki.time = 0;
	input[1].ki.dwExtraInfo = 0;
	input[1].ki.wVk = action->key;
	input[1].ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
	SendInput(2, input, sizeof(INPUT));

	return 1;
}

int Emulator::playMacro(const std::vector<Action*> &actionList) {
	for (Action *action:actionList) {
		std::cout << "action" << std::endl;
		switch (action->type) {
		case TYPE_SLEEP:
			Sleep(action->key);
			break;
		case TYPE_MOUSE:
			if (!doMouseAction(action))
				return 0;
			break;
		case TYPE_KEYBOARD:
			if (!doKeyboardAction(action))
				return 0;
			break;
		default:
			return 0;
		}
	}
	return 1;
}