#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

HICON CreateCursor(int nWidth, int nHeight, COLORREF color)
{
	BITMAPV5HEADER bi = { sizeof(BITMAPV5HEADER) };
	bi.bV5Width = nWidth;
	bi.bV5Height = nHeight;
	bi.bV5Planes = 1;
	bi.bV5BitCount = 32;
	bi.bV5Compression = BI_BITFIELDS;
	bi.bV5RedMask = 0x00FF0000;
	bi.bV5GreenMask = 0x0000FF00;
	bi.bV5BlueMask = 0x000000FF;
	bi.bV5AlphaMask = 0xFF000000;
	HDC hdc = GetDC(0);
	void *lpBits;
	HBITMAP hBitmap = CreateDIBSection(hdc, (LPBITMAPINFO)&bi, DIB_RGB_COLORS, (void **)&lpBits, 0, (DWORD)0);
	HDC hMemDC = CreateCompatibleDC(hdc);
	ReleaseDC(0, hdc);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	RECT rect = { 0, 0, nWidth, nHeight };
	HBRUSH hBrush = CreateSolidBrush(color);
	FillRect(hMemDC, &rect, hBrush);
	DeleteObject(hBrush);
	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
	HBITMAP hMaskBitmap = CreateBitmap(nWidth, nHeight, 1, 1, 0);
	ICONINFO ii = { 0 };
	ii.hbmMask = hMaskBitmap;
	ii.hbmColor = hBitmap;
	HICON hCursor = CreateIconIndirect(&ii);
	DeleteObject(hBitmap);
	DeleteObject(hMaskBitmap);
	return hCursor;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HCURSOR hCurrentCursor;
	static HCURSOR hCursor1;
	static HCURSOR hCursor2;
	switch (msg)
	{
	case WM_CREATE:
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("62"), WS_CHILD | WS_VISIBLE, 10, 10, 50, 30, hWnd, (HMENU)100, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("48"), WS_CHILD | WS_VISIBLE, 62, 10, 50, 30, hWnd, (HMENU)101, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("84"), WS_CHILD | WS_VISIBLE, 10, 50, 50, 30, hWnd, (HMENU)102, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("48"), WS_CHILD | WS_VISIBLE, 62, 50, 50, 30, hWnd, (HMENU)103, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_CHANGE)
		{
			if (LOWORD(wParam) == 100 || LOWORD(wParam) == 101)
			{
				int nWidth = GetDlgItemInt(hWnd, 100, 0, 0);
				int nHeight = GetDlgItemInt(hWnd, 101, 0, 0);
				if (nWidth > 0 && nHeight > 0)
				{
					DestroyIcon(hCursor1);
					hCursor1 = CreateCursor(nWidth, nHeight, RGB(rand() % 256, rand() % 256, rand() % 256));
				}
			}
			else if (LOWORD(wParam) == 102 || LOWORD(wParam) == 103)
			{
				int nWidth = GetDlgItemInt(hWnd, 102, 0, 0);
				int nHeight = GetDlgItemInt(hWnd, 103, 0, 0);
				if (nWidth > 0 && nHeight > 0)
				{
					DestroyIcon(hCursor2);
					hCursor2 = CreateCursor(nWidth, nHeight, RGB(rand() % 256, rand() % 256, rand() % 256));
				}
			}
			hCurrentCursor = hCursor1;
		}
		break;
	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT && (HWND)wParam == hWnd)
		{
			SetCursor(hCurrentCursor);
			return 1;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	case WM_LBUTTONDOWN:
		hCurrentCursor = hCursor2;
		SetCursor(hCurrentCursor);
		break;
	case WM_LBUTTONUP:
		hCurrentCursor = hCursor1;
		SetCursor(hCurrentCursor);
		break;
	case WM_DESTROY:
		DestroyIcon(hCursor1);
		DestroyIcon(hCursor2);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		0,
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("カーソル切り替えテスト"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}