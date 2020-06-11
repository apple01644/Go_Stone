#include "LevelManager.h"
#include "GameManager.h"
#include "Player.h"

#include "LvAbstractStage.h"
#include "LvStart.h"
#include "LvTutorial.h"
#include "LvStage1-1.h"
#include "LvStage1-2.h"
#include "LvStage1-3.h"
#include "LvStage2-1.h"
#include "LvStage2-2.h"
#include "LvStage2-3.h"
#include "LvGameOver.h"
#include "LvRanking.h"

#include <time.h>

HINSTANCE hInst = NULL;
HWND hWnd = NULL;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"TutorialWindowClass";
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	hInst = hInstance;
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	hWnd = CreateWindow(L"TutorialWindowClass", L"Shooting", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL);
	if (!hWnd)
		return E_FAIL;

	ShowWindow(hWnd, nCmdShow);

	return S_OK;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	nCmdShow = 10;
	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return -1;

	mng = new GameManager(hWnd, 640, 480);
	mng->Awake();
	clock_t begin = clock();
	
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			mng->Peek(msg);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			clock_t now = clock();
			mng->deltatime = (now - begin) / (float)CLOCKS_PER_SEC;
			begin = now;
			mng->totaltime += mng->deltatime;
			mng->Loop();
		}
	}

	return (int)msg.wParam;
}