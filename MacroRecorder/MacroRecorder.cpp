// MacroRecorder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

bool readFile(std::string file, std::vector<std::string> &lines) {
	std::string line;
	std::ifstream myfile;
	myfile.open(file);
	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			lines.push_back(line);
		}
		myfile.close();
	}
	else {
		std::cout << "Unable to open macro file";
		return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	MacroCompiler compiler;
	if (argc <= 1) {
		runHooks();
		Recorder rec(&compiler);
		rec.record();
	}
	else {
		Emulator emul;
		std::vector<std::string> lines;
		std::vector<Action*> actionList;
		if (!readFile(argv[1], lines))
			return 1;
		if (compiler.compileMacro(lines, actionList)) {
			std::cout << "Starting playback" << std::endl;
			//emul.playMacro(actionList);
		}
		else {
			std::cout << "Failed to compile macro file!" << std::endl;
		}

	}
	getchar();
    return 0;
}

