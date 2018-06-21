#include "stdafx.h"

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
}

MacroCompiler::~MacroCompiler() {

}

bool MacroCompiler::decompileMacro(std::vector<Action*> &actionList, std::vector<std::string> &textRepresentation) {
	for (auto action : actionList) {
		switch (action->type) {
		case TYPE_SLEEP:
			textRepresentation.push_back("SLEEP " + std::to_string(action->key));
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