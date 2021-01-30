#include "DebugCommandsHelp.h"

#include <iomanip>
#include <sstream>
#include <fstream>
#include <cstring>

#include "../CommandDebugger/CommandDebugger.h"	

unordered_map<string, DebugCommandCode> DebugCommandsHelp::m_fileConstants {
	{"PRINT_BREAK_POINTS", 	PRINT_BREAK_POINTS},
	{"SET_BREAK_POINT", 	SET_BREAK_POINT},
	{"DELETE_BREAK_POINT", 	DELETE_BREAK_POINT},
	{"PRINT_REGISTER", 		PRINT_REGISTER},
	{"SET_REGISTER", 		SET_REGISTER},
	{"PRINT_MEMORY", 		PRINT_MEMORY},
	{"SET_MEMORY", 			SET_MEMORY},
	{"PRINT_COMMAND", 		PRINT_COMMAND},
	{"RUN", 				RUN},
	{"STEP", 				STEP},
	{"QUIT", 				QUIT}
};
array<DebugCommandsHelp::CommandData, HELP> DebugCommandsHelp::m_commandsData;

DebugCommandsHelp::DebugCommandsHelp(DebugCommandsContainer *container) 
	: m_container(container) {
	initCommandsData();
	readConfigFile();
}

void DebugCommandsHelp::initCommandsData() {
	m_commandsData[PRINT_BREAK_POINTS] = {"", "Вывод точек остановы"};
	m_commandsData[SET_BREAK_POINT] =	{"[a]", "Установка точки остановы по адресу \"a\""};
	m_commandsData[DELETE_BREAK_POINT] = {"[a]", "Удаление точки остановы по адресу \"a\""};
	m_commandsData[PRINT_REGISTER] = {"(r) [f]", "Вывод регистра с номером \"r\" в формате \"f\""};
	m_commandsData[SET_REGISTER] = {"(r) (v) [f]", "Присвоение регистру с номером \"r\" значения \"v\" в формате \"f\""};
	m_commandsData[PRINT_MEMORY] = {"(a) [f]", "Вывод участка памяти по адресу \"a\" в формате \"f\""};
	m_commandsData[SET_MEMORY] = {"(a) (v) [f]", "Присвоение участку памяти по адресу \"a\" значения \"v\" в формате \"f\""};
	m_commandsData[PRINT_COMMAND] = {"[a]", "Вывод команды расположенной по адресу \"a\""};
	m_commandsData[RUN] =	{"", "Выполнения программы до следующей точки остановы"};
	m_commandsData[STEP] = {"", "Переход к следующей команде"};
	m_commandsData[QUIT] = {"", "Прекращение отладки"};
}

void DebugCommandsHelp::readConfigFile() {
	ifstream file("config.ini");
	if(file.is_open()) {
		string fileConstant;
		while(!file.eof()) {
			string curLine;
			getline(file, curLine);
			
			stringstream ss(curLine);
			ss >> fileConstant;
			auto code = getCode(fileConstant);
			if(code < COMMANDS_AMOUNT) {				
				rewriteCommand(ss, code);
				rewriteCommandData(ss, code);			
			}
		}
	}
	file.close();
}

DebugCommandCode DebugCommandsHelp::getCode(const string &fileConstant) const {
	auto iter = m_fileConstants.find(fileConstant);
	if(iter != m_fileConstants.end()) {
		return iter->second;
	} else {
		return COMMANDS_AMOUNT;
	}
}

void DebugCommandsHelp::rewriteCommand(istream &is, const DebugCommandCode &code) {
	string alias;
	is >> alias;
	if(alias == "=") { is >> alias; }
	if(!alias.empty()) {
		m_container->setDebugCommand(code, alias);
	}
	
}

inline 
string trim(const string &str) {
	stringstream ss(str), result;
	string tmp;
	ss >> tmp;
	result << tmp;
	while(ss >> tmp) {
		result << " " << tmp;
	}
	return result.str();
}

void DebugCommandsHelp::rewriteCommandData(istream &is, const DebugCommandCode &code) {
	string arguments, description;
	
	getline(is, arguments, '\t');
	getline(is, description);
	
	if(!arguments.empty()) {
		arguments = trim(arguments);
		m_commandsData[code].arguments = arguments;
		if(!description.empty()) {
			m_commandsData[code].description = description;
		} 
	}	
}

unsigned DebugCommandsHelp::countFirstColumnWidth() const {
	unsigned width = 0;
	for(int code = 0; code < HELP; ++code) {
		unsigned curWidth = m_container->getCommand((DebugCommandCode)code).size();
		// '1 + ...' для пробела между командой и аргументами
		curWidth += 1 + m_commandsData[code].arguments.size();	
		if(width < curWidth) {
			width = curWidth;
		}
	}
	return width;
}

void DebugCommandsHelp::printCommand(ostream &os, const DebugCommandCode &code) const {
	const static unsigned WIDTH = countFirstColumnWidth() + 2;
	auto data = m_commandsData[code];
	os << left << setw(WIDTH);
	os << (m_container->getCommand(code) + " " + data.arguments).data() 
	   << data.description << endl;
}

ostream& operator<< (ostream &os, const DebugCommandsHelp &dch) {
	for(int code = 0; code < HELP; ++code) {
		dch.printCommand(os, (DebugCommandCode)code);
	}
	return os;
}

