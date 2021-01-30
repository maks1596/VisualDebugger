#pragma once

#include "Debugger.h"

class DebuggerView {
public:
	virtual void readUserCommands() = 0;
	
	Debugger *getDebugger() {
		return m_debugger;
	}
	void setDebugger(Debugger *debugger) {
		m_debugger = debugger;
	}
	
protected:
	Debugger *m_debugger;	// Указатель на отладчик
};
