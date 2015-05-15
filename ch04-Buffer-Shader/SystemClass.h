#ifndef SYSTEMCLASS_H
#define SYSTEMCLASS_H

//Speed Up Build Process
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass &);
	~SystemClass();

	bool Init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitWindow(int &, int &);
	void ShutdownWindows();

private:
	LPCTSTR AppName;
	HINSTANCE hInstance;
	HWND hWnd;
	InputClass *pInput;
	GraphicsClass *pGraphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass *AppHandle = 0;
#endif  //SystemClass