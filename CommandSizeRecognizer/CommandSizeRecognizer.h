#pragma once

#include "../Define.h"

enum CommandSize {
	SHORT_COMMAND,
	LONG_COMMAND
};

union MemCmd;

class CommandSizeRecognizer {
public:
	static CommandSize getCommandSize(const MemCmd &command);
	static CommandSize getCommandSize(const CommandCode &code);
};
