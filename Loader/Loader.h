#pragma once

#include "../Processor/Processor.h"

class Loader {
public:
	static void load(Processor &processor, const string &name_file);
	
	static void setLoadAddress(const Address &address);
	static Address getLoadAddress();
	
	static size_t getAmountOfLoadedCommands();

private:
	static Address m_loadAddress;
	static size_t m_amountOfLoadedCommands;
};

