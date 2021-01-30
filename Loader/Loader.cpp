#include "Loader.h"

#include <fstream>
#include "../Exception.h"

Address Loader::m_loadAddress = 0;
size_t Loader::m_amountOfLoadedCommands = 0;

void Loader::load(Processor &processor, const string &name_file)
{
	m_amountOfLoadedCommands = 0;
	Address RC = m_loadAddress;
	ifstream file(name_file, ios::binary);

	if (!file.is_open()) {
		throw ExecutableFileNotFound();
	}

	while (!file.eof())
	{
		char prefix = ' ';
		file.read((char *)&prefix, sizeof(prefix));
		if (file.eof()) return;

		switch (prefix)
		{
			case 'k':	// Двоичная команда
			{
				Word data;
				file.read((char *)&data, sizeof(data));

				if (CommandSizeRecognizer::getCommandSize((CommandCode)data.mc.code) == LONG_COMMAND) {
					data.mc.cmd32.a += m_loadAddress;
				}
				processor.m_memory[RC++] = data;
				
				m_amountOfLoadedCommands++;
			} break;
			case 'i':	// Целое число
			case 'f':	// Дробное число
			{
				int argumentsAmount = 0;
				file.read((char *)&argumentsAmount, sizeof(argumentsAmount));
				for (int i = 0; i < argumentsAmount; i++) {
					file.read((char *)&processor.m_memory[RC++], sizeof(Word));
				}
				m_amountOfLoadedCommands++;
			} break;
			case 'a':
			{
				Address adr;
				file.read((char *)&adr, sizeof(adr));
				m_loadAddress = RC = adr;
		m_amountOfLoadedCommands++;
			} break;
			case 's':
			{
				Address adr;
				file.read((char *)&adr, sizeof(adr));
				processor.setIP(m_loadAddress + adr);
			} break;
			default:
			{
				throw LoadingError();
			} break;
		}
	}
	file.close();
}

Address Loader::getLoadAddress() {
	return m_loadAddress;
}
void Loader::setLoadAddress(const Address &address) {
	m_loadAddress = address;
}

size_t Loader::getAmountOfLoadedCommands() {
	return m_amountOfLoadedCommands;
}

