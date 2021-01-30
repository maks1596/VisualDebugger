#include <vector>

namespace VWin32
{
	namespace Data
	{

#ifndef VWin32_Data_DataColumn
#define VWin32_Data_DataColumn
		class DataColumn
		{
		private:
			std::wstring _name;
		public:
			DataColumn(const wchar_t* name)
			{
				this->_name = name;
			}

			void put_Name(wchar_t* name)
			{
				this->_name = name;
			}
			
			std::wstring get_Name()
			{
				return this->_name;
			}
		};
#endif

#ifndef VWin32_Data_DataColumnCollection
#define VWin32_Data_DataColumnCollection
		class DataColumnCollection : public std::vector<DataColumn*>
		{
		public:
			void Add(const wchar_t *columnName)
			{
				this->insert(this->end(), new DataColumn(columnName));
			}
		};
#endif

#ifndef VWin32_Data_DataCell
#define VWin32_Data_DataCell
		class DataCell
		{
		private:
			std::wstring _value;
		public:
			DataCell(const wchar_t* value)
			{
				this->_value = value;
			}

			void put_Value(wchar_t* value)
			{
				this->_value = value;
			}

			std::wstring get_Value()
			{
				return this->_value;
			}
		};
#endif

#ifndef VWin32_Data_DataRow
#define VWin32_Data_DataRow
		class DataRow : public std::vector<DataCell*>
		{
		public:
			void Add(const wchar_t *value)
			{
				this->insert(this->end(), new DataCell(value));
			}

			void Add(VARIANT value)
			{
				std::wstring colName;

				switch (value.vt)
				{
				case VT_BSTR:
					colName = value.bstrVal;
					break;
				case VT_NULL:
					colName = L"";
					break;
				case VT_BOOL:
					colName = Convert::ToWString((bool)value.boolVal);
					break;
				case VT_I4:
					colName = Convert::ToWString(value.lVal);
					break;
				default:
					colName = L"(unknown_type)";
					break;
				}

				this->Add(colName.c_str());
			}
		};
#endif

#ifndef VWin32_Data_DataRowCollection
#define VWin32_Data_DataRowCollection
		class DataRowCollection : public std::vector<DataRow*>
		{
		public:
			DataRow* Add()
			{
				DataRow* row = new DataRow();
				this->insert(this->end(), row);
				return row;
			}
		};
#endif

#ifndef VWin32_Data_DataTable
#define VWin32_Data_DataTable
		class DataTable
		{
		private:
			DataColumnCollection* _columns;
			DataRowCollection* _rows;
		public:

			DataTable()
			{
				this->_columns = new DataColumnCollection();
				this->_rows = new DataRowCollection();
			}

			DataColumnCollection* get_Columns()
			{
				return this->_columns;
			}

			void put_Columns(DataColumnCollection* columns)
			{
				this->_columns = columns;
			}

			DataRowCollection* get_Rows()
			{
				return this->_rows;
			}

			void put_Rows(DataRowCollection* rows)
			{
				this->_rows = rows;
			}
		};
#endif
	}
}

using namespace VWin32::Data;