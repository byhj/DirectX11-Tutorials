#ifndef D3DAPP_H
#define D3DAPP_H

#include <string>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <windows.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "d3dDebug.h"

namespace byhj
{

class D3DApp
{
public:
	D3DApp(){};
	virtual ~D3DApp() {};

	int Run();
	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual bool v_Init();
	virtual void v_Reshape(){};
	virtual void v_Render();
	virtual void v_Update(){};

	virtual void v_MouseDown() {};
	virtual void v_MouseMove(){};
	virtual void v_MouseUp(){};



private:
	bool init_window();
	bool init_d3d();

	HINSTANCE GetAppInst() const;
	HWND      GetHwnd() const;
	float     GetAspect() const;
private:

	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_PosX;
	int m_PosY;

	LPCTSTR m_AppName;
	LPCTSTR m_WndClassName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static byhj::D3DApp *app = NULL;

int D3DApp::Run()
{	
	v_Init();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			if(msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
		   v_Render();
		}

	}

	return (int)msg.wParam;
}

bool D3DApp::v_Init()
{
	if (!init_window())
	   return false;

	if (!init_d3d())
		return false;

	return true;
}

void D3DApp::v_Render()
{

}
bool D3DApp::init_window()
{
	//Window Class information
	m_ScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.75;
	m_ScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.75;
	m_PosX = (GetSystemMetrics(SM_CXSCREEN) - m_ScreenWidth)  / 2;
	m_PosY = (GetSystemMetrics(SM_CYSCREEN) - m_ScreenHeight) / 2;

	WNDCLASSEX wc;	
	wc.cbSize = sizeof(WNDCLASSEX);	
	wc.style = CS_HREDRAW | CS_VREDRAW;	
	//wc.lpfnWndProc = WndProc;	
	wc.cbClsExtra = NULL;	
	wc.cbWndExtra = NULL;	
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);	
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;	
	wc.lpszClassName = m_WndClassName;	
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); 

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Registering Class Failded",	L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	//Create the window and show

	m_hWnd = CreateWindowEx(	
		NULL,	           
		m_WndClassName,	  
		m_AppName,
		WS_OVERLAPPEDWINDOW,	
		m_PosX, m_PosY,
		m_ScreenWidth,	
		m_ScreenHeight,	
		NULL,
		NULL,
		m_hInstance,	
		NULL
		);

	if (!m_hWnd )	
	{
		MessageBox(NULL, L"Creating Window Failed", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);	

	return true;
}

bool D3DApp::init_d3d()
{
	return true;
}

LRESULT CALLBACK D3DApp::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	} 

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}
	}
}

}

/*
byhj::D3DApp * byhj::D3DApp::app; 
#define CALL_MAIN(a)                                    \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) \
{                                                       \
	a *app = new a;                                     \
	app->Run(app);                                      \
	delete app;                                         \
	return 0;                                           \
}

*/
#endif