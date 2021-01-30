#include <Windows.h>

#include "VWin32.Data.h"

namespace VWin32
{
	namespace GUI
	{
#ifndef VWin32_GUI_DataGrid
#define VWin32_GUI_DataGrid
		class DataGrid
		{
		private:

			int nLeft;
			int nTop;
			int nWidth;
			int nHeight;

			DataTable *dataSource;

		public:
			HWND hWnd;
			HINSTANCE hInst;

			DataGrid(HINSTANCE hInst, HWND parent)
			{
				DataGrid::RegisterWndClass(hInst);

				this->hWnd = CreateWindowEx(
					0,
					_T("VWin32.GUI.DataGrid"),
					_T("Window"),
					WS_CHILD | WS_VISIBLE,
					10, 10,
					200,
					200,
					parent,
					0,
					0,
					0
					);

				this->hInst = hInst;
			}

			static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
			{
				switch (message)
				{
				case WM_DESTROY:
					PostQuitMessage(0);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
					break;
				}

				return 0;
			}

			static ATOM RegisterWndClass(HINSTANCE hInst)
			{
				WNDCLASSEX wcex;
				
				wcex.cbSize = sizeof(WNDCLASSEX);
				wcex.style      = CS_HREDRAW | CS_VREDRAW;
				wcex.lpfnWndProc  = &WndProc;
				wcex.cbClsExtra   = 0;
				wcex.cbWndExtra   = 0;
				wcex.hInstance    = hInst;
				wcex.hIcon      = NULL;
				wcex.hCursor    = LoadCursor(NULL, IDC_ARROW);
				wcex.hbrBackground  = CreateSolidBrush(RGB(171, 171, 171));
				wcex.lpszMenuName   = NULL;
				wcex.lpszClassName  = _T("VWin32.GUI.DataGrid");
				wcex.hIconSm    = NULL;

				return RegisterClassEx(&wcex);
			}

			RECT get_Bounds()
			{
				RECT rect;
				GetWindowRect(this->hWnd, &rect);
				return rect;
			}

			void put_Bounds(int x, int y, int width, int height)
			{
				MoveWindow(this->hWnd, x, y, width, height, true);
			}

			int get_Left()
			{
				return this->get_Bounds().left;
			}

			void put_Left(int x)
			{
				this->put_Bounds(x, -1, -1, -1);
			}

			DataTable* get_DataSource()
			{
				return this->dataSource;
			}

			void put_DataSource(DataTable* table)
			{
				this->dataSource = table;

				DataColumnCollection *columns = table->get_Columns();

				for (unsigned i = 0; i < columns->size(); i++)
                {
					DataColumn *column = (*columns)[i];

					int left = 3 + (3 + 100) * i;

					CreateWindowExW(0, L"Static", column->get_Name().c_str(),
						WS_CHILD | WS_VISIBLE, left, 3, 100, 20,
						hWnd, 0, hInst, 0);
                }

				DataRowCollection *rows = table->get_Rows();

				for (unsigned i = 0; i < rows->size(); i++)
                {
					DataRow *row = (*rows)[i];

					int top = 3 + 3 + 20 + (3 + 20) * i;

					for (unsigned j = 0; j < row->size(); j++)
					{
						DataCell *cell = (*row)[j];

						int left = 3 + (3 + 100) * j;
						
						CreateWindowExW(0, L"Edit", cell->get_Value().c_str(),
							WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
							left, top, 100, 20,
							hWnd, 0, hInst, 0);
					}
                }
			}
		};
#endif
	}
}

using namespace VWin32::GUI;