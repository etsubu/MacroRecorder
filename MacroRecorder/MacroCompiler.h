#pragma once

#include "stdafx.h"

typedef struct Action {
	unsigned char type;
	unsigned char action;
	DWORD key;
	unsigned short x, y;
};

// Compiles text representation of macros to binary format, and decompiles from binary format to text
class MacroCompiler {
public:
	MacroCompiler();
	~MacroCompiler();
	// Transforms binary format macros to text representation
	bool decompileMacro(std::vector<Action*> &actionList, std::vector<std::string> &textRepresentation);
	bool compileMacro(std::vector<std::string> &textRepresentation, std::vector<Action*> &actionList);
private:
	void split(std::string line, std::vector<std::string> &words, char delimiter);
	Action *decodeMouseAction(std::vector<std::string> words);
	Action *decodeKeyboardAction(std::vector<std::string> words);
	std::map<unsigned char, std::string> keyboardActionToText;
	std::map<unsigned char, std::string> mouseActionToText;
	std::map<std::string, unsigned char> textToKeyboardAction;
	std::map<std::string, unsigned char> textToMouseAction;
};