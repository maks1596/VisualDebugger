#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include "../DebugCommandsContainer/DebugCommandsContainer.h"

using namespace std;

/*
Обеспечивает вывод подсказки в консоль.
Может прочесть из файла config.ini замену для подсказки по умолчанию
В процессе чтение переписывает текущие название команд на новые, прочитанные из файла. 
*/
class DebugCommandsHelp {
public:
	DebugCommandsHelp(DebugCommandsContainer *container);
	
	friend ostream& operator<< (ostream &os, const DebugCommandsHelp &dch);
	
private:
	void initCommandsData();
	
	void readConfigFile();
	inline DebugCommandCode getCode(const string &fileConstant) const;
	inline void rewriteCommand(istream &is, const DebugCommandCode &code);
	inline void rewriteCommandData(istream &is, const DebugCommandCode &code);
	
	unsigned countFirstColumnWidth() const;
	
	void printCommand(ostream &os, const DebugCommandCode &commandCode) const;
	
	DebugCommandsContainer *m_container;
	static unordered_map<string, DebugCommandCode> m_fileConstants;
	
	struct CommandData {
		string arguments;
		string description;
	};	
	static array<CommandData, HELP> m_commandsData;
};

