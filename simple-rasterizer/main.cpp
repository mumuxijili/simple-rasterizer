// simple-rasterizer.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "resource.h"
#include "render.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

Render* m_render = new Render();
HWND m_hWnd = NULL;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SIMPLERASTERIZER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    //HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIMPLERASTERIZER));

    MSG msg;

  //  // Main message loop:
  //  while (GetMessage(&msg, nullptr, 0, 0))
  //  {
  //      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
  //      {
  //          TranslateMessage(&msg);
  //          DispatchMessage(&msg);
		//}
  //  }

	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			InvalidateRect(m_hWnd, nullptr, true);
			UpdateWindow(m_hWnd);
		}
	}

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPLERASTERIZER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SIMPLERASTERIZER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   RECT wr = { 0, 0, g_winWidth, g_winHeight };    // set the size, but not the position
   AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, TRUE);    // adjust the size
   m_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, hInstance, nullptr);

   if (!m_hWnd)
   {
      return FALSE;
   }

   ShowWindow(m_hWnd, nCmdShow);
   UpdateWindow(m_hWnd);

   m_render->initRender();

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			BITMAPINFO bmpInfo = { 0 };
			bmpInfo.bmiHeader.biBitCount = 32;
			bmpInfo.bmiHeader.biWidth = g_winWidth;
			bmpInfo.bmiHeader.biHeight = g_winHeight;
			bmpInfo.bmiHeader.biPlanes = 1;
			bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

			//hdc = GetDC(hWnd);
			HDC compatibleDC = CreateCompatibleDC(hdc);
			HBITMAP mBitmap = CreateCompatibleBitmap(hdc, g_winWidth, g_winHeight);

			clock_t start = clock();//paint start clock

			DWORD* frameBuffer = m_render->draw();

			clock_t stop = clock();//paint end clock

			SetDIBits(compatibleDC, mBitmap, 0, g_winHeight, frameBuffer, &bmpInfo, DIB_PAL_COLORS);
			SelectObject(compatibleDC, mBitmap);
			//calculate fps
			double dur = (double)(stop - start) / (double)CLOCKS_PER_SEC;
			int fps = (int)min(1000, 1.0 / dur);
			// draw fps
			char fpsstr[32];
			sprintf_s(fpsstr, "FPS: %d", fps);
			drawLabel(compatibleDC, fpsstr, 1);

			// draw bitmap
			BitBlt(hdc, 0, 0, g_winWidth, g_winHeight, compatibleDC, 0, 0, SRCCOPY);

			EndPaint(hWnd, &ps);
			DeleteObject(mBitmap);
			DeleteDC(compatibleDC);
			ReleaseDC(hWnd, hdc);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		break;
		//½ûÖ¹±³¾°²Á³ý ·ÀÖ¹ÉÁË¸?
	case WM_ERASEBKGND:
		return true;
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
