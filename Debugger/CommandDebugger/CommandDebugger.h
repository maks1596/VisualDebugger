#pragma once

#include <unordered_map>
#include <string>

#include "../DebuggerView.h"
#include "../DebugCommandsHelp/DebugCommandsHelp.h"
#include "../DebugCommandsContainer/DebugCommandsContainer.h"

using namespace std;

/*
Командный отладчик
Принимает команды в текстовом виде с консоли,
обрабатывает и передаёт команду непосредственно отладчику 
*/
class CommandDebugger : public DebuggerView {	
public:
	CommandDebugger();
	CommandDebugger(Debugger *debugger);
	
	void readUserCommands() override;

private:	
	void printBreakPoints(const string &);
	void setBreakPoint(const string &address);
	void deleteBreakPoint(const string &address);
	void printRegister(const string &args);
	void setRegister(const string &args);
	void printMemory(const string &address);
	void setMemory(const string &args);
	void printCommand(const string &address);
	void goToNextBreakPoint(const string &);
	void step(const string &);
	void quit(const string &);
	void help(const string &);

	bool m_readDebugCommand;		// Флаг чтения команды отладчика
	// Хранит в себе команды отладчика в строковом виде
	static DebugCommandsContainer m_debugCommandsContainer;
	// Обеспечивает вывод подсказки для команд отладчика
	static const DebugCommandsHelp m_debugCommandsHelp;
	
	typedef void (CommandDebugger::*debugFunction)(const string &args);
	static array<debugFunction, COMMANDS_AMOUNT> m_debugFunctions;
};
