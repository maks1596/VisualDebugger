#include <iostream>
#include <sstream>
#include <Windows.h>

namespace VWin32
{
#ifndef VWin32_Convert
#define VWin32_Convert
	class Convert
	{
	public:
		static std::string ToStdString(int value)
		{
			std::stringstream ss;
			ss << value;
			std::string str = ss.str();
			return str;
		}

		static std::string ToStdString(const std::wstring value)
		{
			if(value.empty())
				return std::string();
			int size_needed = WideCharToMultiByte(CP_UTF8, 0, &value[0], (int)value.size(), NULL, 0, NULL, NULL);
			std::string strTo(size_needed, 0);
			WideCharToMultiByte(CP_UTF8, 0, &value[0], (int)value.size(), &strTo[0], size_needed, NULL, NULL);
			return strTo;
		}

		static std::wstring ToWString(int value)
		{
			std::wstringstream ss;
			ss << value;
			std::wstring str = ss.str();
			return str;
		}

		static std::wstring ToWString(long value)
		{
			std::wstringstream ss;
			ss << value;
			std::wstring str = ss.str();
			return str;
		}

		static std::wstring ToWString(bool value)
		{
			std::wstring str;

			if (value)
			{
				str = L"True";
			}
			else
			{
				str = L"False";
			}

			return str;
		}

		static VARIANT ToVariant(long value)
		{
			VARIANT var;
			VariantInit(&var);
			var.vt = VT_I4;
			var.lVal = value;
			return var;
		}
	};
#endif

#ifndef VWin32_Utils
#define VWin32_Utils
	class Utils
	{
	public:
		static void MsgBox(const char *text)
		{
			MessageBoxA(0, text, 0, MB_OK);
		}

		static void MsgBox(const wchar_t *text)
		{
			MessageBoxW(0, text, 0, MB_OK);
		}

		static void MsgBox(std::string text)
		{
			MsgBox(text.c_str());
		}

		static void MsgBox(std::wstring text)
		{
			MsgBox(text.c_str());
		}

		static void MsgBox(int text)
		{
			MsgBox(Convert::ToStdString(text));
		}

		static void MsgBox()
		{
			MsgBox("");
		}

		static void ErrorBox(std::string error, std::string error_adv, std::string error_causes)
		{
			std::string str;
			str.append("Ошибка:\n");
			str.append(error + "\n\n");
			str.append("Внутр. ошибка:\n");
			str.append(error_adv + "\n\n");
			str.append("Вероятные причины:\n");
			str.append(error_causes);

			MsgBox(str);
		}

		static void ErrorBox(std::wstring error, std::wstring error_adv, std::wstring error_causes)
		{
			std::wstring str;
			str.append(error);
			str.append(error_adv);;
			str.append(error_causes);

			MsgBox(str);
		}
	};
#endif
}

using namespace VWin32;