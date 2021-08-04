#pragma once

#define GAME_NAME "C_Game"

LRESULT CALLBACK MainWindowProc(_In_ HWND windowHandle, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

DWORD createMainGameWindow(void);

BOOL GameIsAlreadyRunning(void);