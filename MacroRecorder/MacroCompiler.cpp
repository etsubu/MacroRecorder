#include "stdafx.h"

// MacroCompiler constructor. Initializes internal maps
MacroCompiler::MacroCompiler() {
	keyboardActionToText[WM_KEYDOWN] = "KEYDOWN";
	keyboardActionToText[WM_KEYUP] = "KEYUP";
	keyboardActionToText[WM_SYSKEYDOWN] = "SYSKEYDOWN";
	keyboardActionToText[WM_SYSKEYUP] = "SYSKEYUP";

	mouseActionToText[WM_RBUTTONDOWN] = "RBUTTONDOWN";
	mouseActionToText[WM_LBUTTONDOWN] = "LBUTTONDOWN";
	mouseActionToText[WM_RBUTTONUP] = "RBUTTONUP";
	mouseActionToText[WM_LBUTTONUP] = "LBUTTONUP";
	mouseActionToText[WM_MBUTTONDOWN] = "MBUTTONDOWN";
	mouseActionToText[WM_MBUTTONUP] = "MBUTTONUNUP";

	textToKeyboardAction["KEYDOWN"] = 0;
	textToKeyboardAction["KEYUP"] = KEYEVENTF_KEYUP;
	textToKeyboardAction["SYSKEYDOWN"] = 0;
	textToKeyboardAction["SYSKEYUP"] = KEYEVENTF_KEYUP;

	textToMouseAction["RBUTTONDOWN"] = MOUSEEVENTF_RIGHTDOWN;
	textToMouseAction["LBUTTONDOWN"] = MOUSEEVENTF_LEFTDOWN;
	textToMouseAction["RBUTTONUP"] = MOUSEEVENTF_RIGHTUP;
	textToMouseAction["LBUTTONUP"] = MOUSEEVENTF_LEFTUP;
	textToMouseAction["MBUTTONDOWN"] = MOUSEEVENTF_MIDDLEDOWN;
	textToMouseAction["MBUTTONUNUP"] = MOUSEEVENTF_MIDDLEUP;
}

// MacroCompiler destructor
MacroCompiler::~MacroCompiler() {

}

// Splits string and stores the parts to a vector
void MacroCompiler::split(std::string line, std::vector<std::string> &words, char delimiter) {
	int index = 0;
	while ((index = line.find(delimiter)) != -1) {
		words.push_back(line.substr(0, index));
		if (index + 1 == line.length())
			return;
		line = line.substr(index + 1);
	}
	if (!line.empty())
		words.push_back(line);
}

// Decompiles a macro from binary format to text representation
bool MacroCompiler::decompileMacro(std::vector<Action*> &actionList, std::vector<std::string> &textRepresentation) {
	for (auto action : actionList) {
		switch (action->type) {
		case TYPE_SLEEP:
			textRepresentation.push_back("SLEEP:" + std::to_string(action->key));
			break;
		case TYPE_MOUSE:
			if (mouseActionToText.count(action->action) == 0)
				return false;
			textRepresentation.push_back(mouseActionToText[action->action + 0x200] + ":" + std::to_string(action->x) + ":" + std::to_string(action->y));
			break;
		case TYPE_KEYBOARD:
			if (keyboardActionToText.count(action->action) == 0)
				return false;
			textRepresentation.push_back(keyboardActionToText[action->action + 0x100] + ":" + std::to_string(action->x) + ":" + std::to_string(action->y) + ":" + std::to_string(action->key));
			break;
		default:
			return false;
		}
	}
	return true;
}

Action* MacroCompiler::decodeMouseAction(std::vector<std::string> words) {
	int x, y;
	try  {
		x = std::stoi(words[1], nullptr, 10);
		y = std::stoi(words[2], nullptr, 10);
	}
	catch (const std::exception &e) {
		return nullptr;
	}
	Action *a = new Action;
	a->x = x;
	a->y = y;
	a->key = 0;
	a->type = TYPE_MOUSE;
	a->action = textToMouseAction[words[0]];
	return a;
}

Action* MacroCompiler::decodeKeyboardAction(std::vector<std::string> words) {
	if (words.size() < 4)
		return nullptr;
	int x, y, key;
	try {
		x = std::stoi(words[1], nullptr, 10);
		y = std::stoi(words[2], nullptr, 10);
		key = std::stoi(words[3], nullptr, 10);
	}
	catch (const std::exception &e) {
		return nullptr;
	}
	Action *a = new Action;
	a->x = x;
	a->y = y;
	a->key = key;
	a->type = TYPE_KEYBOARD;
	a->action = textToKeyboardAction[words[0]];
	return a;
}

// Compiles a macro from text format to binary format
bool MacroCompiler::compileMacro(std::vector<std::string> &textRepresentation, std::vector<Action*> &actionList) {
	for (std::string line : textRepresentation) {
		if (line.empty())
			continue;
		if (line[0] == '#')
			continue;
		std::vector<std::string> splitted;
		split(line, splitted, ':');
		if (splitted[0].find("SLEEP") == 0 && splitted.size() == 2) {
			try {
				int delay = std::stoi(splitted[1], nullptr, 10);
				Action *a = new Action;
				a->x = 0;
				a->y = 0;
				a->key = delay;
				a->type = TYPE_SLEEP;
				a->action = 0;
				actionList.push_back(a);
			}
			catch (const std::exception &e) {
				return nullptr;
			}
			continue;
		}
		if (splitted.size() < 3)
			return false;
		Action *action;
		if (textToMouseAction.count(splitted[0]) > 0) {
			action = decodeMouseAction(splitted);
		}
		else if (textToKeyboardAction.count(splitted[0]) > 0) {
			action = decodeKeyboardAction(splitted);
		}
		else {
			return false;
		}
		if (action == nullptr) {
			return false;
		}
		actionList.push_back(action);
	}
	return true;
}