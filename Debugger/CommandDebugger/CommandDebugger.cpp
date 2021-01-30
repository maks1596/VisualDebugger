#include "CommandDebugger.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "../../Processor/Processor.h"
#include "../../Exception.h"

DebugCommandsContainer CommandDebugger::m_debugCommandsContainer;
const DebugCommandsHelp CommandDebugger::m_debugCommandsHelp(&m_debugCommandsContainer);
array<CommandDebugger::debugFunction, COMMANDS_AMOUNT> CommandDebugger::m_debugFunctions;

CommandDebugger::CommandDebugger() {	
	m_debugFunctions[PRINT_BREAK_POINTS] = &CommandDebugger::printBreakPoints;
	m_debugFunctions[SET_BREAK_POINT] = &CommandDebugger::setBreakPoint;
	m_debugFunctions[DELETE_BREAK_POINT] = &CommandDebugger::deleteBreakPoint;
	m_debugFunctions[PRINT_REGISTER] = &CommandDebugger::printRegister;
	m_debugFunctions[SET_REGISTER] = &CommandDebugger::setRegister;
	m_debugFunctions[PRINT_MEMORY] = &CommandDebugger::printMemory;
	m_debugFunctions[SET_MEMORY] = &CommandDebugger::setMemory;
	m_debugFunctions[PRINT_COMMAND] = &CommandDebugger::printCommand;
	m_debugFunctions[RUN] = &CommandDebugger::goToNextBreakPoint;
	m_debugFunctions[STEP] = &CommandDebugger::step;
	m_debugFunctions[QUIT] = &CommandDebugger::quit;
	m_debugFunctions[HELP] = &CommandDebugger::help;
	m_debugFunctions[QUESTION_SIGN] = &CommandDebugger::help;
}

CommandDebugger::CommandDebugger(Debugger *debugger) :
	CommandDebugger() 
{
	DebuggerView::setDebugger(debugger);
}

void CommandDebugger::readUserCommands() {
	m_readDebugCommand = true;
	while (m_readDebugCommand) {
		printCommand("");   // Выводим текущую команду
		cout << "dbg> ";
		string debugCommand;
		cin >> debugCommand;
		auto code = m_debugCommandsContainer.getCode(debugCommand);
		if (code < COMMANDS_AMOUNT) {
			string args;
			getline(cin, args);
			try {
				(this->*m_debugFunctions.at(code))(args);
			} // Ловим ошибки парсинга и собственные исключения
			catch (const exception &ex) {
				cerr << ex.what() << endl;
			}
		} else {
			cerr << "Неизвестная команда отладчика" << endl;
		}
	}
}

void CommandDebugger::printBreakPoints(const string &) {
	auto breakPoints = m_debugger->getBreakPoints();
	if(!breakPoints.empty()) {
		for(const auto &bp : breakPoints) {
			cout << bp << " ";
		}
	} else {
		cout << "Точки остановы не установлены";
	}
	cout << endl;
}
void CommandDebugger::setBreakPoint(const string &strBreakPoint) {
	Address breakPoint;
	if(strBreakPoint.empty()) {
		m_debugger->setBreakPointByCurrentAddress();
	} else {
		breakPoint = (Address)stoul(strBreakPoint);
		m_debugger->setBreakPoint(breakPoint);
	}
}
void CommandDebugger::deleteBreakPoint(const string &strBreakPoint) {
	if (strBreakPoint.empty()) {
		m_debugger->deleteAllBreakPoints();
	} else {
		Address breakPoint = (Address)stoul(strBreakPoint);
		m_debugger->deleteBreakPoint(breakPoint);
	}
	
}

void CommandDebugger::printRegister(const string &args) {
	stringstream ss(args);
	string strRegNumber, strFormat;
	ss >> strRegNumber >> strFormat;

	if(strRegNumber.empty()) {
		throw NotEnoughArguments();
	}

	unsigned regNumber = (unsigned)stoul(strRegNumber);
	if(regNumber >= REGISTERS_AMOUNT) {
		throw InvalidRegisterNumber();
	}

	char format = '\0';
	if(!strFormat.empty()) { format = strFormat[0]; }

	switch(format) {
		case 'u': {
			cout << "r" << regNumber << " = " << m_debugger->getUnsignedRegisterValue(regNumber);
		} break;
		case 'i': {
			cout << "r" << regNumber << " = " << m_debugger->getIntegerRegisterValue(regNumber);
		} break;
		case 'f': {
			cout << "r" << regNumber << " = " << m_debugger->getFloatRegisterValue(regNumber);
		} break;
		case '\0': {
			cout << "r" << regNumber << " = " << hex << m_debugger->getRegisterValue(regNumber);
		} break;
		default: {
			throw UnknownFormat();
		} break;
	}
	cout << endl;
}
void CommandDebugger::setRegister(const string &args) {
	stringstream ss(args);
	string strRegNumber, strValue, strFormat;
	ss >> strRegNumber >> strValue >> strFormat;

	if(strRegNumber.empty() || strValue.empty()) {
		throw NotEnoughArguments();
	}

	unsigned regNumber = (unsigned)stoul(strRegNumber);
	if(regNumber >= REGISTERS_AMOUNT) {
		throw InvalidRegisterNumber();
	}

	char format = 'i';
	if(!strFormat.empty()) { format = strFormat[0]; }
	
	switch(format) {
		case 'u': {
			m_debugger->setRegisterValue(regNumber, (unsigned)stoul(strValue));
		} break;
		case 'i': {
			m_debugger->setRegisterValue(regNumber, stoi(strValue));
		} break;
		case 'f': {
			m_debugger->setRegisterValue(regNumber, stof(strValue));
		} break;
		default: {
			throw UnknownFormat();
		} break;
	}
}

void CommandDebugger::printMemory(const string &args) {
	stringstream ss(args);
	string strAddress, strFormat;
	ss >> strAddress >> strFormat;

	if(strAddress.empty()) {
		throw NotEnoughArguments();
	}
	Address address = (Address)stoul(strAddress);

	char format = '\0';
	if(!strFormat.empty()) { format = strFormat[0]; }

	switch(format) {
		case 'u': {
			cout << "Memory[" << address << "] = " << m_debugger->getUnsignedMemory(address);
		} break;
		case 'i': {
			cout << "Memory[" << address << "] = " << m_debugger->getIntegerMemory(address);
		} break;
		case 'f': {
			cout << "Memory[" << address << "] = " << m_debugger->getFloatMemory(address);
		} break;
		case '\0': {
			cout << "Memory[" << address << "] = " << hex << m_debugger->getMemory(address);
		} break;
		default: {
			throw UnknownFormat();
		} break;
	}
	cout << endl;
}
void CommandDebugger::setMemory(const string &args) {
	stringstream ss(args);
	string strAddress, strValue, strFormat;
	ss >> strAddress >> strValue >> strFormat;

	if(strAddress.empty() || strValue.empty()) {
		throw NotEnoughArguments();
	}

	Address address = (Address)stoul(strAddress);
	char format = 'i';
	if(!strFormat.empty()) { format = strFormat[0]; }
	
	switch(format) {
		case 'u': {
			m_debugger->setMemory(address, (unsigned)stoul(strValue));
		} break;
		case 'i': {
			m_debugger->setMemory(address, stoi(strValue));
		} break;
		case 'f': {
			m_debugger->setMemory(address, stof(strValue));
		} break;
		default: {
			throw UnknownFormat();
		} break;
	}
}

void CommandDebugger::printCommand(const string &strAddress) {
	MemCmd command;
	Address address;
	if(strAddress.empty()) {
		command = m_debugger->getCurrentCommand();
		address = m_debugger->getCurrentAddress();
	} else {
		Address address = (Address)stoul(strAddress);
		command = m_debugger->getCommand(address);
	}
	
	cout << address << ": ";
	CommandSize size = CommandSizeRecognizer::getCommandSize(command);
	switch (size) {
		case SHORT_COMMAND:	cout << command.fcmd16 << endl;	break;
		case LONG_COMMAND:	cout << command.cmd32 << endl;	break;
	}
}

void CommandDebugger::goToNextBreakPoint(const string &) {
	m_readDebugCommand = false;
}

void CommandDebugger::step(const string &) {
	m_debugger->step();
	m_readDebugCommand = false;
}

void CommandDebugger::quit(const string &) {
	m_debugger->stopProgram();
	m_readDebugCommand = false;
}

void CommandDebugger::help(const string &) {
	cout << m_debugCommandsHelp;
}

