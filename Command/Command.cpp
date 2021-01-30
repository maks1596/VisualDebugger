#include "command.h"

#include <iostream>
#include "../Processor/Processor.h"



void StopCmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->m_psw.Stop = 1;
}

//  �������� ����� 
void AddICmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2].i += processor->RON[cmd.fcmd16.r1].i;
}

//  ��������� ����� 
void SubICmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2].i -= processor->RON[cmd.fcmd16.r1].i;
}

//  ��������� ����� 
void MulICmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2].i *= processor->RON[cmd.fcmd16.r1].i;
}

//  ������� ����� 
void DivICmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2].i /= processor->RON[cmd.fcmd16.r1].i;
}

//  ������� �� ������� ����� 
void ModICmd::execute(const MemCmd &cmd, Processor *processor) {
	processor->RON[cmd.fcmd16.r2].i %= processor->RON[cmd.fcmd16.r1].i;
}


//  ��������� ����������� ����� 
void MulUICmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2].u *= processor->RON[cmd.fcmd16.r1].u;
}

//  ������� ����������� ����� 
void DivUICmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2].u /= processor->RON[cmd.fcmd16.r1].u;
}

//  ������� �� ������� ����������� ����� 
void ModUICmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2].u %= processor->RON[cmd.fcmd16.r1].u;
}


//  �������� ������� 
void AddFCmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2].f += processor->RON[cmd.fcmd16.r1].f;
}

//  ��������� ������� 
void SubFCmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2].f -= processor->RON[cmd.fcmd16.r1].f;
}

//  ��������� ������� 
void MulFCmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2].f *= processor->RON[cmd.fcmd16.r1].f;
}

//  ������� ������� 
void DivFCmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2].f = processor->RON[cmd.fcmd16.r1].f / processor->RON[cmd.fcmd16.r2].f;
}

//  ��������� 
void MovCmdRR::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.fcmd16.r2] = processor->RON[cmd.fcmd16.r1];
}
void MovCmdRM::execute(const MemCmd &cmd, Processor *processor)
{	
	processor->m_memory[cmd.cmd32.a] = processor->RON[cmd.cmd32.r2];
}
void MovCmdMR::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.cmd32.r2] = processor->m_memory[cmd.cmd32.a];
}

//  ��������� ����� 
void CmpICmd::execute(const MemCmd &cmd, Processor *processor)
{
	if (processor->RON[cmd.fcmd16.r2].i == processor->RON[cmd.fcmd16.r1].i)
		processor->m_psw.RF = EQUAL;
	else if (processor->RON[cmd.fcmd16.r2].i > processor->RON[cmd.fcmd16.r1].i)
		processor->m_psw.RF = GREATER;
	else if (processor->RON[cmd.fcmd16.r2].i < processor->RON[cmd.fcmd16.r1].i)
		processor->m_psw.RF = LOWER;
}

//  ��������� ����� ����������� 
void CmpUICmd::execute(const MemCmd &cmd, Processor *processor)
{
	if (processor->RON[cmd.fcmd16.r2].u == processor->RON[cmd.fcmd16.r1].u)
		processor->m_psw.RF = EQUAL;
	else if (processor->RON[cmd.fcmd16.r2].u > processor->RON[cmd.fcmd16.r1].u)
		processor->m_psw.RF = GREATER;
	else if (processor->RON[cmd.fcmd16.r2].u < processor->RON[cmd.fcmd16.r1].u)
		processor->m_psw.RF = LOWER;
}

// ��������� ������� 
void CmpFCmd::execute(const MemCmd &cmd, Processor *processor)
{
	if (processor->RON[cmd.fcmd16.r2].f == processor->RON[cmd.fcmd16.r1].f)
		processor->m_psw.RF = EQUAL;
	else if (processor->RON[cmd.fcmd16.r2].f > processor->RON[cmd.fcmd16.r1].f)
		processor->m_psw.RF = GREATER;
	else if (processor->RON[cmd.fcmd16.r2].f < processor->RON[cmd.fcmd16.r1].f)
		processor->m_psw.RF = LOWER;
}

// ���� ����� 
void InICmd::execute(const MemCmd &cmd, Processor *processor)
{
	std::cin.clear();
	std::wcout << L"Ввод целого число: ";
	std::cin >> processor->RON[cmd.fcmd16.r1].i;
}

// ����� ����� 
void OutICmd::execute(const MemCmd &cmd, Processor *processor)
{
	std::wcout << L"Вывод целого числа: ";
	std::cout << processor->RON[cmd.fcmd16.r1].i << std::endl;
}

// ���� ������� 
void InFCmd::execute(const MemCmd &cmd, Processor *processor)
{
	std::cin.clear();
	std::wcout << L"Ввод дробного числа: ";
	std::cin >> processor->RON[cmd.fcmd16.r1].f;
}

// ����� ������� 
void OutFCmd::execute(const MemCmd &cmd, Processor *processor)
{
	std::wcout << L"Вывод дробного числа: ";
	std::cout << processor->RON[cmd.fcmd16.r1].f << std::endl;
}

// ��������
void JmpCmd::execute(const MemCmd &cmd, Processor *processor)
{
	switch (cmd.cmd32.r1) {
		case 1: {
			processor->setIP(cmd.cmd32.a);
			processor->m_psw.JF = 1;
		} break;
		case 2: {
			Address ip = processor->getIP();
			processor->setIP(ip + cmd.cmd32.r2 + cmd.cmd32.a);
			processor->m_psw.JF = 1;
		} break;
		case 3: {
			processor->setIP(cmd.cmd32.r2 + cmd.cmd32.a);
			processor->m_psw.JF = 1;
		} break;
	}
}

//  ������� "�����" 
void JeCmd::execute(const MemCmd &cmd, Processor *processor)
{
	if (processor->m_psw.RF == EQUAL)
		JmpCmd::execute(cmd, processor);
}

//  ������� "������" 
void JlCmd::execute(const MemCmd &cmd, Processor *processor)
{
	if (processor->m_psw.RF == LOWER)
		JmpCmd::execute(cmd, processor);
}

//  ������� "������" 
void JgCmd::execute(const MemCmd &cmd, Processor *processor)
{
	if (processor->m_psw.RF == GREATER)
		JmpCmd::execute(cmd, processor);
}

//  ������� � ������������ 
void CallCmd::execute(const MemCmd &cmd, Processor *processor)
{
	processor->RON[cmd.cmd32.r1].u = processor->getIP() + 1;
	processor->m_returnAddress = cmd.cmd32.r1;
	JmpCmd::execute(cmd, processor);
}

//  ������� � ���������� ������������ 
void RetCmd::execute(const MemCmd &cmd, Processor *processor)
{
	MemCmd jmpCmd = MemCmd();
	jmpCmd.cmd32.code = CommandCode::Jmp;	// ������ ������
	jmpCmd.cmd32.r1 = 1;					// ������
	jmpCmd.cmd32.a = (Address)processor->RON[processor->m_returnAddress].u; // �� ������

	JmpCmd::execute(jmpCmd, processor);
}
