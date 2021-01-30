#ifndef COMMANDS_H
#define COMMANDS_H

#include "../Define.h"

class Processor;
union MemCmd;

struct Command
{
public:
	void operator ()(const MemCmd &cmd, Processor *processor) {
		execute(cmd, processor);
	}
	virtual void execute(const MemCmd &cmd, Processor *processor) = 0;
};
class NOPCmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor) {};
};
class StopCmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor);
};
class AddICmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class SubICmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class MulICmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor); 
};

class MulUICmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class DivICmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor); 
};

class DivUICmd : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class ModICmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor); 
};
class ModUICmd : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class AddFCmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor); 
};

class SubFCmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class MulFCmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor); 
};

class DivFCmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor); 
};

class MovCmdRR : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class MovCmdRM : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};


class MovCmdMR : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class CmpICmd : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class CmpUICmd : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class CmpFCmd : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class InICmd : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class OutICmd : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class InFCmd : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class OutFCmd : public Command
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class JmpCmd : public Command
{
public:
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class JeCmd : public JmpCmd
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class JlCmd : public JmpCmd
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class JgCmd : public JmpCmd
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class CallCmd : public JmpCmd
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

class RetCmd : public JmpCmd
{
	virtual void execute(const MemCmd &cmd, Processor *processor);
};

#endif // COMMANDS_H
