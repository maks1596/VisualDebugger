#include <iostream>
#include <iomanip>
#include <cstring>
#include <sstream>
#include <functional>
#include <Windows.h>
#include <tchar.h>
#include <Commctrl.h>
#include <regex>

using namespace std;

#include "Processor/Processor.h"
#include "Debugger/Debugger.h"
#include "Debugger/CommandDebugger/CommandDebugger.h"
#include "Loader/Loader.h"
#include "Exception.h"
#include "Define.h"
#include "CommandSizeRecognizer/CommandSizeRecognizer.h"


const string UNSIGNED_REGEXP(R"((\d|([1-9]\d*)))");
const string SIGNED_REGEXP(R"((\+|-))" + UNSIGNED_REGEXP);
const string FLOAT_REGEXP(R"((\+|-)?)" + UNSIGNED_REGEXP + R"(\.\d+)");


Word MEMORY[MEMORY_SIZE];
Processor PROCESSOR(MEMORY);
Debugger DEBUGGER(&PROCESSOR);
TCHAR LAST_FILE[260];

//------------ Дескрипторы контролов ------------
#define ID_CODE_LIST 0
HWND hCodeList;

#define IDC_LBL2 1
HWND hWndLbl2;

#define ID_REG_NUM_VAL 2
HWND hRegNumVal;
#define ID_REG_NUM_UD 3
HWND hRegNumUD;

#define ID_UINT_LBL 4
HWND hUIntLbl;
#define ID_UINT_VAL 5
HWND hUIntVal;

#define ID_INT_LBL 6
HWND hIntLbl;
#define ID_INT_VAL 7
HWND hIntVal;

#define ID_FLOAT_LBL 8
HWND hFloatLbl;
#define ID_FLOAT_VAL 9
HWND hFloatVal;

#define IDC_LBL5 10
HWND hWndLbl5;

#define ID_NEW_REG_VAL 11
HWND hNewRegVal;

#define ID_CHANGE_REG_VAL_BTN 12
HWND hChangeRegValBtn;

#define ID_STEP_BTN 13
HWND hStepBtn;

#define ID_CONTINUE_BTN 14
HWND hContinueBtn;

#define ID_STOP_BTN 15
HWND hStopBtn;

#define ID_LOAD_BTN 16
HWND hLoadBtn;

#define ID_RELOAD_BTN 17
HWND hReloadBtn;

//------------- КОНЕЦ: IDы контролов ------------

void updateCodeList() {
	SendMessage(hCodeList, LB_RESETCONTENT, 0, 0);
	
	for(Address cur = Loader::getLoadAddress(); 
		cur < Loader::getAmountOfLoadedCommands(); 
		cur++) {
		stringstream ss;
		if(cur == DEBUGGER.getCurrentAddress()) {
			if(DEBUGGER.isBreakPoint(cur)) {
				ss << "*> ";
			} else {
				ss << "-> ";
			}
		} else if (DEBUGGER.isBreakPoint(cur)){
			ss << "*  ";
		} else {
			ss << "   ";
		}
		ss << setw(3) << cur << "| ";
		 
		ss << DEBUGGER.getCommand(cur);
		SendMessage(hCodeList, LB_ADDSTRING, 0, (LPARAM)ss.str().data());
	}
}

void loadFile(const string &fileName) {
	PROCESSOR.reset();
	Loader::load(PROCESSOR, fileName);
	updateCodeList();	
}

void reloadFile() {
	try {
		loadFile(LAST_FILE);
	} catch(const exception &ex) {
		MessageBox(0, TEXT(ex.what()), 0, MB_ICONERROR);
	}
}

void openFile() {
	OPENFILENAME ofn;
   	
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = LAST_FILE;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(LAST_FILE);
	ofn.lpstrFilter = "Файл ВМ\0*.vmexec\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
   	
   	if(GetOpenFileName(&ofn) == TRUE) {
		reloadFile();
	}
}

inline
bool isUnsigned(TCHAR *regValue) {
	regex reg(UNSIGNED_REGEXP);
	return regex_match(regValue, reg);
}

inline
bool isSigned(TCHAR *regValue) {
	regex reg(SIGNED_REGEXP);
	return regex_match(regValue, reg);
}

inline
bool isFloat(TCHAR *regValue) {
	regex reg(FLOAT_REGEXP);
	return regex_match(regValue, reg);
}

template<typename T>
const char *ttoa(const T &val) {
	stringstream ss;
	ss << val;
	return ss.str().data();
}

template<typename T>
T atot(const char *a) {
	if(!a) { return T();	}
	
	stringstream ss(a);
	T val;
	ss >> val;
	return val;
}

inline
void updateRegValue(const unsigned &num) {	
	SetWindowText(hUIntVal, ttoa(DEBUGGER.getUnsignedRegisterValue(num)));
	SetWindowText(hIntVal, ttoa(DEBUGGER.getIntegerRegisterValue(num)));
	SetWindowText(hFloatVal, ttoa(DEBUGGER.getFloatRegisterValue(num)));
}

template<typename T>
void setRegisterValue(const unsigned &num, const T &val) {
	DEBUGGER.setRegisterValue(num, val);
	updateRegValue(num);
	SetWindowText(hNewRegVal, "\0");
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
	case WM_VSCROLL:
      	if ((HWND)(lParam) == hRegNumUD
          	&& LOWORD(wParam) == SB_ENDSCROLL) {
          	TCHAR regNum[10];
			GetWindowText(hRegNumVal, regNum, sizeof(regNum));
			
			if(regNum) {
				updateRegValue(atoi(regNum));
			}
		}
    //---------- События контролов ----------
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {	
        
        case ID_CODE_LIST: {
        	switch(HIWORD(wParam)) {
        		case LBN_ERRSPACE: {
        			MessageBox(hWnd, "Мало памяти", "", MB_OK);
				} break;
        		case LBN_DBLCLK: {
        			int uItem = (int) SendMessage(hCodeList, LB_GETCURSEL, 0, 0L);
					if(uItem != LB_ERR) {
	        			Address bp = Loader::getLoadAddress() + uItem;
	        			if(DEBUGGER.isBreakPoint(bp)) {
	        				DEBUGGER.deleteBreakPoint(bp);
						} else {
							DEBUGGER.setBreakPoint(bp);
						}
						updateCodeList();
					}					
				} break;
			} 
		} break;
				
        case ID_CHANGE_REG_VAL_BTN: {
            if (HIWORD(wParam) == BN_CLICKED) {
            	TCHAR regNum[10];
				GetWindowText(hRegNumVal, regNum, sizeof(regNum));
				int num = atoi(regNum);
            	
            	TCHAR regValue[10];
				GetWindowText(hNewRegVal, regValue, sizeof(regValue));
				if(isUnsigned(regValue)) { 	
					unsigned val = atot<unsigned>(regValue);
					setRegisterValue(num, val);
				} else if(isSigned(regValue)) { 
					int val = atot<int>(regValue);
					setRegisterValue(num, val);
				} else if(isFloat(regValue)) { 
					float val = atot<float>(regValue);
					setRegisterValue(num, val);
				}				
            }
        } break;
        
        case ID_STEP_BTN: {
        	if(HIWORD(wParam) == BN_CLICKED) {
        		DEBUGGER.step();
        		updateCodeList();
			}
		} break;
		
		case ID_CONTINUE_BTN: {
        	if(HIWORD(wParam) == BN_CLICKED) {
        		DEBUGGER.run();
        		updateCodeList();
			}
		} break;
		
		case ID_STOP_BTN: {
        	if(HIWORD(wParam) == BN_CLICKED) {
        		DEBUGGER.stopProgram();
        		updateCodeList();
			}
		} break;
		
		case ID_LOAD_BTN: {
        	if(HIWORD(wParam) == BN_CLICKED) {
        		openFile();
			}
		} break;
		
		case ID_RELOAD_BTN: {
        	if(HIWORD(wParam) == BN_CLICKED) {
        		reloadFile();
			}
		} break;
	
        // ----------------------------------
        default:
            break;
        }
        break;
    //------ КОНЕЦ: События контролов -------
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    
    return 0;
}

HWND CreateWin(HINSTANCE hInst,int nCmdShow) {
    WNDCLASSEX wcex;
    
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style      = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc  = WndProc;
    wcex.cbClsExtra   = 0;
    wcex.cbWndExtra   = 0;
    wcex.hInstance    = hInst;
    wcex.hIcon      = NULL;
    wcex.hCursor    = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)COLOR_WINDOW;
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = TEXT("MainWndClass");
    wcex.hIconSm    = NULL;
    
    RegisterClassEx(&wcex);
    
    int FrameTitleH = GetSystemMetrics(SM_CYCAPTION);
    int FrameBorderW = GetSystemMetrics(SM_CXSIZEFRAME);

    //---------------- Создание окна ----------------
    HWND hWnd = CreateWindowEx(
        0,
        TEXT("MainWndClass"),
        TEXT("Отладчик"),
        WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
        200, 300,
        709 + FrameBorderW * 2,
        382 + FrameTitleH + FrameBorderW * 2,
        0,
        0,
        0,
        0
    );
    //------------ КОНЕЦ: Создание окна ------------
    
    //------------- Создание элементов -------------
    if (hWnd != NULL) 
    {
        hCodeList = CreateWindowEx(
            0,
            TEXT("ListBox"),
            NULL,
            WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
            12, 12,
            530, 302,
            hWnd,
            (HMENU)ID_CODE_LIST,
            hInst,
            0
        );
        HFONT hCodeListFont = CreateFont (-MulDiv(12, GetDeviceCaps(GetDC(0), LOGPIXELSY), 72), 0, 0, 0, FW_NORMAL, false, false, false, ANSI_CHARSET, 
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
            DEFAULT_PITCH | FF_SWISS, _T("Consolas"));
        SendMessage (hCodeList, WM_SETFONT, (WPARAM)hCodeListFont, 1);

        hWndLbl2 = CreateWindowEx(
            0,
            TEXT("Static"),
            TEXT("Регистр"),
            WS_VISIBLE | WS_CHILD,
            622, 30,
            57, 14,
            hWnd,
            (HMENU)IDC_LBL2,
            hInst,
            0
        );
        
        hRegNumVal = CreateWindowEx(
            0,
            TEXT("Edit"),
            TEXT("0"),
            WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_LEFT | WS_BORDER,
            570, 30, 40, 15,
            hWnd,
            (HMENU)ID_REG_NUM_VAL,
            hInst,
            0
        );

        hRegNumUD = CreateWindowEx(
            0,
            UPDOWN_CLASS,
            NULL,
            WS_CHILD | WS_VISIBLE | WS_TABSTOP
            | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT
            | UDS_SETBUDDYINT | WS_BORDER,
            0, 0, 0, 0,
            hWnd,
            (HMENU)ID_REG_NUM_UD,
            hInst,
            0
        );
        SendMessage(hRegNumUD, UDM_SETBUDDY, (WPARAM)hRegNumVal, 0L);
        SendMessage(hRegNumUD, UDM_SETRANGE, 0, MAKELPARAM(REGISTERS_AMOUNT - 1, 0));   
		SendMessage(hRegNumUD, UDM_SETPOS, 0L, MAKELONG(0, 0));
		
		hUIntLbl = CreateWindowEx(
            0,
            TEXT("Static"),
            TEXT("Беззнаковое:"),
            WS_VISIBLE | WS_CHILD,
            558, 50, 131, 15,
            hWnd,
            (HMENU)ID_UINT_LBL,
            hInst,
            0
		);
		hUIntVal = CreateWindowEx(
            0,
            TEXT("Static"),
            TEXT("0"),
            WS_VISIBLE | WS_CHILD,
            660, 50, 30, 15,
            hWnd,
            (HMENU)ID_UINT_VAL,
            hInst,
            0
		);
		
		hIntLbl = CreateWindowEx(
            0,
            TEXT("Static"),
            TEXT("Знаковое:"),
            WS_VISIBLE | WS_CHILD,
            558, 75, 131, 15,
            hWnd,
            (HMENU)ID_INT_LBL,
            hInst,
            0
		);

		hIntVal = CreateWindowEx(
            0,
            TEXT("Static"),
            TEXT("+0"),
            WS_VISIBLE | WS_CHILD,
            660, 75, 30, 15,
            hWnd,
            (HMENU)ID_INT_VAL,
            hInst,
            0
		);
		
		hFloatLbl = CreateWindowEx(
            0,
            TEXT("Static"),
            TEXT("Дробное:"),
            WS_VISIBLE | WS_CHILD,
            558, 100, 131, 14,
            hWnd,
            (HMENU)ID_FLOAT_LBL,
            hInst,
            0
		);

		hFloatVal = CreateWindowEx(
            0,
            TEXT("Static"),
            TEXT("0.0"),
            WS_VISIBLE | WS_CHILD,
            660, 100, 30, 15,
            hWnd,
            (HMENU)ID_FLOAT_VAL,
            hInst,
            0
		);

        hWndLbl5 = CreateWindowEx(
            0,
            TEXT("Static"),
            TEXT("Новое значение"),
            WS_VISIBLE | WS_CHILD,
            558, 175, 131, 14,
            hWnd,
            (HMENU)IDC_LBL5,
            hInst,
            0
        );

        hNewRegVal = CreateWindowEx(
            0,
            TEXT("Edit"),
            TEXT("0"),
            WS_VISIBLE | WS_CHILD,
            564, 200,
            118, 17,
            hWnd,
            (HMENU)ID_NEW_REG_VAL,
            hInst,
            0
        );

        hChangeRegValBtn = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("Записать"),
            WS_VISIBLE | WS_CHILD,
            612, 231,
            73, 21,
            hWnd,
            (HMENU)ID_CHANGE_REG_VAL_BTN,
            hInst,
            0
        );

        hStepBtn = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("Сделать шаг"),
            WS_VISIBLE | WS_CHILD,
            28, 325,
            107, 21,
            hWnd,
            (HMENU)ID_STEP_BTN,
            hInst,
            0
        );

        hContinueBtn = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("Продолжить выполнение"),
            WS_VISIBLE | WS_CHILD,
            174, 325,
            173, 21,
            hWnd,
            (HMENU)ID_CONTINUE_BTN,
            hInst,
            0
        );

        hStopBtn = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("Остановить отладку"),
            WS_VISIBLE | WS_CHILD,
            384, 325,
            152, 20,
            hWnd,
            (HMENU)ID_STOP_BTN,
            hInst,
            0
        );
        
        hLoadBtn = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("Загрузить файл"),
            WS_VISIBLE | WS_CHILD,
            28, 350,
            180, 20,
            hWnd,
            (HMENU)ID_LOAD_BTN,
            hInst,
            0
        );
        
        hReloadBtn = CreateWindowEx(
            0,
            TEXT("Button"),
            TEXT("Перезапустить"),
            WS_VISIBLE | WS_CHILD,
            220, 350,
            150, 20,
            hWnd,
            (HMENU)ID_RELOAD_BTN,
            hInst,
            0
        );
    }
    //---------- КОНЕЦ: Создание элементов ---------
    
    return hWnd;
}

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow) {
    HWND hWnd = CreateWin(hInst,nCmdShow);
    if (hWnd == NULL) {
        MessageBox(0, TEXT("Не удается создать окно."), 0, MB_ICONERROR);
        return 1;
    } else {
        ShowWindow(hWnd, nCmdShow);
    }
	
	openFile();
	
	MSG msg;
    while (GetMessage(&msg,NULL,0,0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}

