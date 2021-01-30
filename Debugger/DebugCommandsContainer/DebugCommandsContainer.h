#pragma once

#include <string>
#include <array>

using namespace std;

enum DebugCommandCode {
	PRINT_BREAK_POINTS,
	SET_BREAK_POINT,
	DELETE_BREAK_POINT,
	PRINT_REGISTER,
	SET_REGISTER,
	PRINT_MEMORY,
	SET_MEMORY,
	PRINT_COMMAND,
	RUN,
	STEP,
	QUIT,
	HELP,			// должна быть всегда после команд, 
	QUESTION_SIGN,	// для котрых требуется выводить подсказки
	COMMANDS_AMOUNT	// должна быть после всех команд
};

class DebugCommandsContainer {
public:
	DebugCommandsContainer();
	
	DebugCommandCode getCode(const string &com) const;
	string getCommand(const DebugCommandCode &code) const;
	void setDebugCommand(const DebugCommandCode &code, const string &com);
	
private:
	static array<string, COMMANDS_AMOUNT> m_debugCommands;
};

