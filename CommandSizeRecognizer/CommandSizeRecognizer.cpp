#include "CommandSizeRecognizer.h"

#include "..\Exception.h"
#include "..\Processor\Processor.h"

CommandSize CommandSizeRecognizer::getCommandSize(const MemCmd &command) {
	getCommandSize((CommandCode)command.code);
}

CommandSize CommandSizeRecognizer::getCommandSize(const CommandCode &code) {
	switch(code)	{
		case Nop:
		case Stop:
		case AddI:
		case SubI:
		case MulI:
		case MulUI:
		case DivI: 
		case DivUI:
		case ModI:
		case ModUI:
		case AddF:
		case SubF:
		case MulF:
		case DivF:
		case CmpI:
		case CmpUI:
		case CmpF:
		case InI:
		case OutI:
		case InF:
		case OutF:
		case MovRR:	return SHORT_COMMAND;
		case MovRM:
		case MovMR:		
		case Jmp:
		case Je:
		case Jl:
		case Jg:
		case Call:
		case Ret:	return LONG_COMMAND;
		default:	throw UnknownCommand();
	}
}
