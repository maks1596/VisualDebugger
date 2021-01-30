#pragma once
#include <iostream>
#include <iomanip>

#include "../Define.h"
#include "../Command/Command.h"
#include "../CommandSizeRecognizer/CommandSizeRecognizer.h"

using namespace std;

// Короткая команда. Занимает полслова - 16 бит
struct MemCmd16 {
	HalfWord code : 6;
	HalfWord r1 : 5;
	HalfWord r2 : 5;
	
	friend ostream& operator<< (ostream& os, const MemCmd16 &command);
};

// Длинная команда. Занимает целое слово - 32 бита
// Наследуется от короткой команды для упрощения доступа к общим полям
struct MemCmd32 : public MemCmd16 {
	Address a;
	
	friend ostream& operator<< (ostream& os, const MemCmd32 &command);
};


union MemCmd {
	// code выделен для упрощения доступа к коду первой короткой команды
	// или единственной длинной команды
	HalfWord code : 6;
	
	struct {
		MemCmd16 fcmd16;
		MemCmd16 scmd16;
	};
	MemCmd32 cmd32;
	
	friend ostream& operator<< (ostream& os, const MemCmd &command);
};


// Слово - 32 бита
union Word {
	int i;
	unsigned u;
	float f;
	MemCmd mc;
	Word() : i() {}
};

// Значения объединённого флага результата (см. PSW ниже)
enum ResultFlagValue : Byte {
	EQUAL,
	LOWER,
	GREATER
};

struct PSW {	
	HalfWord JF : 1;	// Флаг перехода
	HalfWord RF : 2;	// Флаги результата
	HalfWord Stop : 1;	// Флаг остановки (завершения работы ВМ)
	
private:
	HalfWord reserve : 12;
};

class Processor {
public:
	Processor(Word *memory);
	void interpreter();
	void executeCommand();
	void reset();
	
	Address getIP() const;
	void setIP(const Address &address);
	
	MemCmd getCurrentCommand() const;
	
	PSW m_psw;						// Состояние процессора
	Word RON[REGISTERS_AMOUNT];		// Регистры общего назначения
	Word *m_memory;					// Указатель на память
	Address m_returnAddress;		// Адрес возврата из подпрограммы

private:
	void initCommands();			// Инициализация команд
	void resetFlags();				// Сброс всех флагов
	
	Address m_IP;					// Текущий адрес
	MemCmd m_currentCommand;		// Текущая команда
	Command *commands[64];			// Исполняемые команды
};

