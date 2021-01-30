#include <initguid.h>
#include <string>
#include "adoid.h"
#include "adoint.h"

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

#include "VWin32.h"
#include "VWin32.Data.h"
#include "VWin32.Runtime.InteropServices.h"

namespace VWin32
{
	namespace Data
	{
		namespace OleDb
		{
			class OleDbConnection
			{
			private:
				_ADOConnection* _pConn;
			public: 
				OleDbConnection(const char* connectionString)
				{
					HRESULT hr;

					CoInitialize(NULL);

					hr = CoCreateInstance(CLSID_CADOConnection, NULL, CLSCTX_INPROC_SERVER, IID_IADOConnection, (void**)&_pConn);
					if (FAILED(hr))
					{
						Utils::ErrorBox("�� ������� ������� OleDbConnection",
							"CoCreateInstance(CLSID_CADOConnection...) FAILED",
							"");
						return;
					}

					BSTR conStr = Marshal::StringToBSTR(connectionString);
					hr = _pConn->put_ConnectionString(conStr);
					if (FAILED(hr))
					{
						Utils::ErrorBox("�� ������� ������� OleDbConnection",
							"put_ConnectionString FAILED",
							"");
					}
				}

				void Open()
				{
					HRESULT hr;
					
					hr = _pConn->Open();
					if (FAILED(hr))
					{
						Utils::ErrorBox("�� ������� ������������ � �� ����� OleDbConnection.",
							"_pConn->Open() FAILED",
							"- �������� ���� � ����� ��.\n- ������� ������ ���������.\n- �� ���������� ������ ���������.\n- ���������� ��������� �� ��� �����������, ��� ������� �������������� ����������.\n- �������� � ����� ����������� ������ �����������.");
					}
				}

				void Close()
				{
					HRESULT hr;
					
					hr = _pConn->Close();
					if (FAILED(hr))
					{
						Utils::ErrorBox("�� ������� ����������� �� � �� ����� OleDbConnection.",
							"_pConn->Close() FAILED",
							"- ������ OleDbConnection ���� ������� � ������� ������ Open(), ����� ��������� ����������� �������, ����� ���� �������.");
					}
				}

				ADOConnection* ToCOMNative()
				{
					return this->_pConn;
				}
			};

			class OleDbCommand
			{
			private:
				_ADOCommand* _pCmd;
			public:
				OleDbCommand(const char* query, OleDbConnection* conn)
				{
					HRESULT hr;

					hr = CoCreateInstance(CLSID_CADOCommand, NULL, CLSCTX_INPROC_SERVER, IID_IADOCommand, (void**)&_pCmd);
					if (FAILED(hr))
					{
						Utils::ErrorBox("�� ������� ������� OleDbCommand",
							"CoCreateInstance(CLSID_CADOCommand...) FAILED",
							"");
						return;
					}

					VARIANT activeConn;
					VariantInit(&activeConn);
					activeConn.vt = VT_DISPATCH;
					activeConn.pdispVal = (IDispatch*) conn->ToCOMNative();

					hr = _pCmd->put_ActiveConnection(activeConn);
					if (FAILED(hr))
					{
						Utils::ErrorBox("�� ������� ������� OleDbCommand",
							"_pCmd->put_ActiveConnection(activeConn) FAILED",
							"���������� OleDbConnection �� �������. �������� ����� Open(), ����� ���� ����������� � ��.");
						return;
					}

					BSTR bstrCmdText = Marshal::StringToBSTR(query);
					hr = this->_pCmd->put_CommandText(bstrCmdText);
					if (FAILED(hr))
					{
						Utils::ErrorBox("�� ������� ������� OleDbCommand",
							"this->_pCmd->put_CommandText FAILED",
							"");
						return;
					}
				}

				int ExecuteNonQuery()
				{
					VARIANT rowsAffected;
					VariantInit(&rowsAffected);
					rowsAffected.vt = VT_INT;

					_ADORecordset *result;
					
					CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_INPROC_SERVER, IID_IADORecordset, (void**)&result);

					HRESULT hr = _pCmd->Execute(&rowsAffected, NULL, adCmdText, &result);

					if (FAILED(hr))
					{
						Utils::ErrorBox("�� ������� ��������� SQL-������ OleDbCommand",
							"_pCmd->Execute FAILED",
							"������ � ���������� SQL-�������.");
					}

					return rowsAffected.intVal;
				}

				ADOCommand* ToCOMNative()
				{
					return this->_pCmd;
				}
			};

			class OleDbDataAdapter
			{
			private:
				_ADORecordset* rs;
			public: 
				OleDbDataAdapter(const char* selectQuery, OleDbConnection* conn)
				{
					HRESULT hr;

					hr = CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_INPROC_SERVER, IID_IADORecordset, (void**)&rs);
					
					BSTR cmdText = Marshal::StringToBSTR(selectQuery);
					hr = conn->ToCOMNative()->Execute(cmdText, NULL, adCmdText, &rs);
				}

				void Fill(DataTable *table)
				{
					HRESULT hr;

					ADOFields *fields;
					
					hr = rs->get_Fields(&fields);

					long count;
					fields->get_Count(&count);

					for (int i = 0; i < count; i++)
					{
						ADOField *field;

						hr = fields->get_Item(Convert::ToVariant(i), &field);

						BSTR bstrColName;
						hr = field->get_Name(&bstrColName);

						std::wstring colName(bstrColName);

						table->get_Columns()->Add(colName.c_str());
					}

					/* Rows */

					rs->MoveFirst();

					while (true) {

						VARIANT_BOOL bbb;

						rs->get_EOF(&bbb);

						if (bbb)
							break;

						DataRow *row = table->get_Rows()->Add();

						for (int i = 0; i < count; i++)
						{
							ADOField *field;

							hr = fields->get_Item(Convert::ToVariant(i), &field);

							VARIANT val;

							hr = field->get_Value(&val);

							row->Add(val);
						}

						rs->MoveNext();
					}
				}
				
				ADORecordset* ToCOMNative()
				{
					return this->rs;
				}
			};
		}
	}
}

using namespace VWin32::Data::OleDb;