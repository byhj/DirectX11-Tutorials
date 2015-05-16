#ifndef SYSTEMCLASS_H
#define SYSTEMCLASS_H

//Speed up the build process
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass &rhs);
	~SystemClass();

	//member function
	bool Init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	//call by member function
	void Render();
	void InitWindow(int &, int &);
	void ShutdownWindow();

private:
	LPCTSTR AppName;
	HINSTANCE hInstance;
	HWND hWnd;

	InputClass *pInput;
	GraphicsClass *pGraphics;
};

//re-direct the windows system messaging into our MessageHandler function
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass *AppHandle = 0;

#endif   //system class