#include "DebugCommandsContainer.h"

#include <algorithm>

array<string, COMMANDS_AMOUNT> DebugCommandsContainer::m_debugCommands;

DebugCommandsContainer::DebugCommandsContainer() {
	m_debugCommands[PRINT_BREAK_POINTS] = "bp";
	m_debugCommands[SET_BREAK_POINT] = "set-bp";
	m_debugCommands[DELETE_BREAK_POINT] = "del-bp";
	m_debugCommands[PRINT_REGISTER] = "reg";
	m_debugCommands[SET_REGISTER] = "set-reg";
	m_debugCommands[PRINT_MEMORY] = "mem";
	m_debugCommands[SET_MEMORY] = "set-mem";
	m_debugCommands[PRINT_COMMAND] = "com";
	m_debugCommands[RUN] = "run";
	m_debugCommands[STEP] = "step";
	m_debugCommands[QUIT] = "quit";
	m_debugCommands[HELP] = "help";
	m_debugCommands[QUESTION_SIGN] = "?";
}

DebugCommandCode DebugCommandsContainer::getCode(const string &com) const {
	auto iter = find(m_debugCommands.cbegin(), m_debugCommands.cend(), com);
	return (DebugCommandCode) (iter - m_debugCommands.cbegin());
}


string DebugCommandsContainer::getCommand(const DebugCommandCode &code) const {
	return m_debugCommands[code];
}

void DebugCommandsContainer::setDebugCommand(const DebugCommandCode &code, const string &com) {
	m_debugCommands[code] = com;
}

