#pragma once

#include <exception>
using namespace std;

class ExecutableFileNotFound : public exception {
public:
	virtual const char * what() const noexcept {
		return "Исполняемый файл не найден";
	}
};

class LoadingError : public exception {
public:
	virtual const char * what() const noexcept {
		return "Ошибка загрузки исполняемого файла";
	}
};

class NotEnoughArguments : public exception {
public:
	virtual const char * what() const noexcept {
		return "Недостаточно аргументов";
	}
};

class InvalidRegisterNumber : public exception {
public:
	virtual const char * what() const noexcept {
		return "Недопустимый номаер регистра";
	}
};

class UnknownFormat : public exception {
public:
	virtual const char * what() const noexcept {
		return "Неизвестный формат";
	}
};

class UnknownCommandLineParameters : public exception {
public:
	virtual const char * what() const noexcept {
		return "Неизвестный параметр командной строки";
	}
};

class UnknownCommand : public exception {
public:
	virtual const char * what() const noexcept {
		return "Неизвестная команда";
	}
};

