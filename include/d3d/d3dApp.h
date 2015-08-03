#ifndef D3DAPP_H
#define D3DAPP_H

#include <string>
#define WIN32_LEAN_AND_MEAN

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <WindowsX.h>
#include <windows.h>
#include <d3dx11.h>
#include <dxgi.h>
#include <d3d11.h>
#include <xnamath.h>
#include <d3dcommon.h>
#include <D3DX11async.h>

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
	D3DApp() :m_AppName(L"Framework"), m_WndClassName(L"D3DWindow")
	{

	}
	virtual ~D3DApp() {}

	void InitApp();
	int Run();
	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual	void v_Init()  = 0;
	virtual void v_Shutdown() = 0;
	virtual void v_Render()   = 0;
	virtual void v_Update() {}

	// Convenience overrides for handling mouse input.
	virtual void v_OnMouseDown(WPARAM btnState, int x, int y){ }
	virtual void v_OnMouseUp(WPARAM btnState, int x, int y)  { }
	virtual void v_OnMouseMove(WPARAM btnState, int x, int y){ }
	virtual void v_OnMouseWheel(WPARAM btnState, int x, int y) { }

protected:
	int   m_ScreenWidth;
	int   m_ScreenHeight;
	float m_ScreenFar;
	float m_ScreenNear;
	int   m_PosX;
	int   m_PosY;

	LPCTSTR m_AppName;
	LPCTSTR m_WndClassName;

	//void      GetVideoCardInfo(char &, int &);
	HINSTANCE GetAppInst() const { return m_hInstance; }
	HWND      GetHwnd()    const { return m_hWnd; }
	float     GetAspect()  const { return (float)m_ScreenWidth / m_ScreenHeight; }

private:
	bool init_window();

private:
	HINSTANCE m_hInstance;
	HWND      m_hWnd;
};

}
#endif