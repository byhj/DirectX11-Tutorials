#ifndef SYSTEMCLASS_H
#define SYSTEMCLASS_H

//Speed Up Build Process
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "InputClass.h"
#include "GraphicsClass.h"

#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
#include "PositionClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass &);
	~SystemClass();

	//Member function
	bool Init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	//Call by member function
	bool Render();
	void InitWindow(int &, int &);
	void ShutdownWindow();

private:
	LPCTSTR AppName;
	HINSTANCE hInstance;
	HWND hWnd;

	InputClass    *pInput;
	GraphicsClass *pGraphics;

	FpsClass      *pFps;
	CpuClass      *pCpu;
	TimerClass    *pTimer;
	PositionClass *pPosition;

	int screenW, screenH;
	int posX, posY;
};

//re-direct the windows system messaging into our MessageHandler function 
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass *AppHandle = 0;

#endif  //SystemClass