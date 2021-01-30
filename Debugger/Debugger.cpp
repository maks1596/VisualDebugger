#include "Debugger.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

#include "../Processor/Processor.h"
#include "DebuggerView.h"


void Debugger::step() {
	if(!m_processor->m_psw.Stop) {
		m_processor->executeCommand();
	}
}

void Debugger::run() {
	while (!m_processor->m_psw.Stop && 
		!isBreakPoint(m_processor->getIP())) {
		m_processor->executeCommand();	
	}	
}

void Debugger::stopProgram() {
	m_processor->m_psw.Stop = 1;
}

void Debugger::setProcessor(Processor *processor) {
	m_processor = processor;
}
Processor *Debugger::getProcessor() {
	return m_processor;
}

bool Debugger::isBreakPoint(const Address &address) const {
	return (find(m_breakPoints.begin(), m_breakPoints.end(), address) != m_breakPoints.end());
}

const BreakPoints& Debugger::getBreakPoints() const {
	return m_breakPoints;
}

void Debugger::setBreakPointByCurrentAddress() {
	setBreakPoint(m_processor->getIP());
}

void Debugger::setBreakPoint(const Address &address) {
	m_breakPoints.push_back(address);
}

void Debugger::deleteBreakPoint(const Address &address) {
	auto bpIter = find(m_breakPoints.begin(), m_breakPoints.end(), address);
	m_breakPoints.erase(bpIter);	// Если не нашли точку остановы, то, впринципе, и не важно
}

void Debugger::deleteAllBreakPoints() {
	m_breakPoints.clear();
}

unsigned Debugger::getRegisterValue(const unsigned &regNumber) const {
	return getUnsignedRegisterValue(regNumber);
}

unsigned Debugger::getUnsignedRegisterValue(const unsigned &regNumber) const {
	return m_processor->RON[regNumber].u;
}

int Debugger::getIntegerRegisterValue(const unsigned &regNumber) const {
	return m_processor->RON[regNumber].i;
}

float Debugger::getFloatRegisterValue(const unsigned &regNumber) const {
	return m_processor->RON[regNumber].f;
}

void Debugger::setRegisterValue(const unsigned &regNumber, const unsigned &value) {
	m_processor->RON[regNumber].u = value;
}

void Debugger::setRegisterValue(const unsigned &regNumber, const int &value) {
	m_processor->RON[regNumber].i = value;
}

void Debugger::setRegisterValue(const unsigned &regNumber, const float &value) {
	m_processor->RON[regNumber].f = value;
}

unsigned Debugger::getMemory(const Address &address) const {
	return getUnsignedMemory(address);
}

unsigned Debugger::getUnsignedMemory(const Address &address) const {
	return m_processor->m_memory[address].u;
}

int Debugger::getIntegerMemory(const Address &address) const {
	return m_processor->m_memory[address].i;
}

float Debugger::getFloatMemory(const Address &address) const {
	return m_processor->m_memory[address].f;
}

void Debugger::setMemory(const Address &address, const unsigned &value) {
	m_processor->m_memory[address].u = value;
}

void Debugger::setMemory(const Address &address, const int &value) {
	m_processor->m_memory[address].i = value;
}

void Debugger::setMemory(const Address &address, const float &value) {
	m_processor->m_memory[address].f = value;
}

Address Debugger::getCurrentAddress() const {
	return m_processor->getIP();
}

MemCmd Debugger::getCurrentCommand() const {
	return m_processor->getCurrentCommand();
}

MemCmd Debugger::getCommand(const Address &address) const {
	return m_processor->m_memory[address].mc;
}

