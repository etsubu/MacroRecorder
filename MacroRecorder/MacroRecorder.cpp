// MacroRecorder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc, char *argv[])
{
	MacroCompiler compiler;
	runHooks();
	Recorder rec(&compiler);
	rec.record();
	std::string s;
	std::getline(std::cin, s);
	std::getline(std::cin, s);
	std::getline(std::cin, s);
    return 0;
}

