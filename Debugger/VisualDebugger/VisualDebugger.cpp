//#include <Windows.h>
//#include <tchar.h>
//#include <Commctrl.h>
//
////------------ Дескрипторы контролов ------------
//HWND hWndLst1;
//#define IDC_LST1 0
//HWND hWndLbl2;
//#define IDC_LBL2 1
//HWND hWndCmb3;
//#define IDC_CMB3 2
//HWND hWndTre4;
//#define IDC_TRE4 3
//HWND hWndLbl5;
//#define IDC_LBL5 4
//HWND hWndTxt6;
//#define IDC_TXT6 5
//HWND hWndBtn7;
//#define IDC_BTN7 6
//HWND hWndBtn8;
//#define IDC_BTN8 7
//HWND hWndBtn9;
//#define IDC_BTN9 8
//HWND hWndBtn10;
//#define IDC_BTN10 9
////------------- КОНЕЦ: IDы контролов ------------
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    //---------- События контролов ----------
//    case WM_COMMAND:
//        switch (LOWORD(wParam))
//        {
//
//        case IDC_BTN7:
//            if (HIWORD(wParam) == BN_CLICKED)
//            {
//                MessageBox(0, _T("Кнопка нажата"), _T(""), 0);
//            }
//            break;
//
//        // ----------------------------------
//        default:
//            break;
//        }
//        break;
//    //------ КОНЕЦ: События контролов -------
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//        break;
//    }
//    
//    return 0;
//}
//
//HWND CreateWin(HINSTANCE hInst,int nCmdShow)
//{
//    WNDCLASSEX wcex;
//    
//    wcex.cbSize = sizeof(WNDCLASSEX);
//    wcex.style      = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc  = WndProc;
//    wcex.cbClsExtra   = 0;
//    wcex.cbWndExtra   = 0;
//    wcex.hInstance    = hInst;
//    wcex.hIcon      = NULL;
//    wcex.hCursor    = LoadCursor(NULL, IDC_ARROW);
//    wcex.hbrBackground  = (HBRUSH)COLOR_WINDOW;
//    wcex.lpszMenuName   = NULL;
//    wcex.lpszClassName  = _T("MainWndClass"); //класс окна
//    wcex.hIconSm    = NULL;
//    
//    RegisterClassEx(&wcex);
//    
//    int FrameTitleH = GetSystemMetrics(SM_CYCAPTION);
//    int FrameBorderW = GetSystemMetrics(SM_CXSIZEFRAME);
//
//    //---------------- Создание окна ----------------
//    HWND hWnd = CreateWindowEx(
//        0,
//        _T("MainWndClass"),
//        _T("Отладчик"),
//        WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
//        200, 300,
//        709 + FrameBorderW * 2,
//        382 + FrameTitleH + FrameBorderW * 2,
//        0,
//        0,
//        0,
//        0
//    );
//    //------------ КОНЕЦ: Создание окна ------------
//    
//    //------------- Создание элементов -------------
//    if (hWnd != NULL) 
//    {
//        hWndLst1 = CreateWindowEx(
//            0,
//            _T("ListBox"),
//            _T("Lst1"),
//            WS_VISIBLE | WS_CHILD,
//            12, 12,
//            530, 302,
//            hWnd,
//            (HMENU)IDC_LST1,
//            hInst,
//            0
//        );
//
//        hWndLbl2 = CreateWindowEx(
//            0,
//            _T("Static"),
//            _T("Регистр"),
//            WS_VISIBLE | WS_CHILD,
//            622, 33,
//            57, 14,
//            hWnd,
//            (HMENU)IDC_LBL2,
//            hInst,
//            0
//        );
//
//        hWndCmb3 = CreateWindowEx(
//            0,
//            _T("ComboBox"),
//            _T("Cmb3"),
//            WS_VISIBLE | WS_CHILD | CBS_DROPDOWN,
//            562, 30,
//            49, 14,
//            hWnd,
//            (HMENU)IDC_CMB3,
//            hInst,
//            0
//        );
//
//        hWndTre4 = CreateWindowEx(
//            0,
//            _T("SysTreeView32"),
//            _T("Tre4"),
//            WS_VISIBLE | WS_CHILD,
//            559, 61,
//            133, 91,
//            hWnd,
//            (HMENU)IDC_TRE4,
//            hInst,
//            0
//        );
//
//        hWndLbl5 = CreateWindowEx(
//            0,
//            _T("Static"),
//            _T("Новое значение"),
//            WS_VISIBLE | WS_CHILD,
//            558, 175,
//            131, 14,
//            hWnd,
//            (HMENU)IDC_LBL5,
//            hInst,
//            0
//        );
//
//        hWndTxt6 = CreateWindowEx(
//            0,
//            _T("Edit"),
//            _T("0x"),
//            WS_VISIBLE | WS_CHILD,
//            564, 200,
//            118, 17,
//            hWnd,
//            (HMENU)IDC_TXT6,
//            hInst,
//            0
//        );
//
//        hWndBtn7 = CreateWindowEx(
//            0,
//            _T("Button"),
//            _T("Записать"),
//            WS_VISIBLE | WS_CHILD,
//            612, 231,
//            73, 21,
//            hWnd,
//            (HMENU)IDC_BTN7,
//            hInst,
//            0
//        );
//
//        hWndBtn8 = CreateWindowEx(
//            0,
//            _T("Button"),
//            _T("Сделать шаг"),
//            WS_VISIBLE | WS_CHILD,
//            28, 328,
//            107, 21,
//            hWnd,
//            (HMENU)IDC_BTN8,
//            hInst,
//            0
//        );
//
//        hWndBtn9 = CreateWindowEx(
//            0,
//            _T("Button"),
//            _T("Продолжить выполнение"),
//            WS_VISIBLE | WS_CHILD,
//            174, 326,
//            173, 21,
//            hWnd,
//            (HMENU)IDC_BTN9,
//            hInst,
//            0
//        );
//
//        hWndBtn10 = CreateWindowEx(
//            0,
//            _T("Button"),
//            _T("Остановить отладку"),
//            WS_VISIBLE | WS_CHILD,
//            384, 325,
//            152, 20,
//            hWnd,
//            (HMENU)IDC_BTN10,
//            hInst,
//            0
//        );
//
//
////        HFONT hLbl5Font = CreateFont (-MulDiv(8, GetDeviceCaps(GetDC(0), LOGPIXELSY), 72), 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, 
////            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
////            DEFAULT_PITCH | FF_SWISS, _T("Microsoft Sans Serif"));
////        
////        SendMessage (hWndLbl5, WM_SETFONT, (WPARAM)hLbl5Font, 1);
//    }
//    //---------- КОНЕЦ: Создание элементов ---------
//    
//    return hWnd;
//}
//
//int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow)
//{
//    HWND hWnd = CreateWin(hInst,nCmdShow);
//    if (hWnd == NULL)
//    {
//        MessageBox(0, _T("Не удается создать окно."), 0, MB_ICONERROR);
//        return 1;
//    }
//    else
//    {
//        ShowWindow(hWnd, nCmdShow);
//    }
//    
//    MSG msg;
//    while (GetMessage(&msg,NULL,0,0))
//    {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//    
//    return 0;
//}

