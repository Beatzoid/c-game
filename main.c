#include <stdio.h>

#pragma warning (push)
#pragma warning (disable: 4668)
#pragma warning (disable: 5105)

#include <windows.h>

#pragma warning(pop)

#include "main.h"

int WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ PSTR commandLine, _In_ INT cmdShow)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(cmdShow);

	if (GameIsAlreadyRunning() == TRUE)
	{
		MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);

		goto exit;
	}

	if (createMainGameWindow() != ERROR_SUCCESS)
	{
		goto exit;
	}

	MSG message = { 0 };

	while (GetMessageA(&message, NULL, 0, 0) > 0)
	{
		TranslateMessage(&message);

		DispatchMessageA(&message);
	}

	exit:
		return 0;
}

LRESULT CALLBACK MainWindowProc(
	_In_ HWND windowHandle, 
	_In_ UINT message, 
	_In_ WPARAM wParam, 
	_In_ LPARAM lParam)
{
	LRESULT result = 0;

	switch (message)
	{
		case WM_CLOSE:
			PostQuitMessage(0);

			break;

		default:
			result = DefWindowProcA(windowHandle, message, wParam, lParam);
	}

	return result;
}


DWORD createMainGameWindow(void)
{
	DWORD result = ERROR_SUCCESS;

	WNDCLASSEXA windowClass = { 0 };
	HWND windowHandle = 0;

	windowClass.cbSize = sizeof(WNDCLASSEX);

	windowClass.style = 0;

	windowClass.lpfnWndProc = MainWindowProc;

	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;

	windowClass.hInstance = GetModuleHandleA(NULL);

	windowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
	windowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);

	windowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = GAME_NAME "_WINDOW_CLASS";

	if (RegisterClassExA(&windowClass) == 0)
	{
		result = GetLastError();
		MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

		goto exit;
	}

	windowHandle = CreateWindowExA(
		WS_EX_CLIENTEDGE,
		windowClass.lpszClassName,
		GAME_NAME,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, GetModuleHandleA(NULL), NULL);

	if (windowHandle == NULL)
	{
		result = GetLastError();
		MessageBoxA(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

		goto exit;
	}

	exit:
		return result;
}


BOOL GameIsAlreadyRunning(void)
{
	HANDLE muteX = NULL;

	muteX = CreateMutexA(NULL, FALSE, GAME_NAME "_Game_Mutex");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
