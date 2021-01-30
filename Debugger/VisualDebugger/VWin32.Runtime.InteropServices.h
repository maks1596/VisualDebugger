#include <iostream>

#include <Windows.h>

#include "VWin32.h"

namespace VWin32
{
	namespace Runtime
	{
		namespace InteropServices
		{
			class Marshal
			{
			public: static BSTR StringToBSTR(const char *str)
				{
					int wslen = MultiByteToWideChar(CP_ACP, 0, str, strlen(str), 0, 0);
					BSTR bstr = SysAllocStringLen(0, wslen);
					MultiByteToWideChar(CP_ACP, 0, str, strlen(str), bstr, wslen);
					return bstr;
				}

			public: static BSTR StringToBSTR(const wchar_t *str)
				{
					BSTR bstr = SysAllocString(str);
					return bstr;
				}

			public: static BSTR StringToBSTR(std::string str)
				{
					return StringToBSTR(str.c_str());
				}

			public: static BSTR StringToBSTR(std::wstring str)
				{
					return StringToBSTR(str.c_str());
				}
			};
		}
	}
}

using namespace VWin32::Runtime::InteropServices;