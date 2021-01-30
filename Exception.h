#pragma once

#include <exception>
using namespace std;

class ExecutableFileNotFound : public exception {
public:
	virtual const char * what() const noexcept {
		return "����������� ���� �� ������";
	}
};

class LoadingError : public exception {
public:
	virtual const char * what() const noexcept {
		return "������ �������� ������������ �����";
	}
};

class NotEnoughArguments : public exception {
public:
	virtual const char * what() const noexcept {
		return "������������ ����������";
	}
};

class InvalidRegisterNumber : public exception {
public:
	virtual const char * what() const noexcept {
		return "������������ ������ ��������";
	}
};

class UnknownFormat : public exception {
public:
	virtual const char * what() const noexcept {
		return "����������� ������";
	}
};

class UnknownCommandLineParameters : public exception {
public:
	virtual const char * what() const noexcept {
		return "����������� �������� ��������� ������";
	}
};

class UnknownCommand : public exception {
public:
	virtual const char * what() const noexcept {
		return "����������� �������";
	}
};

