#ifndef D3DINPUT_H
#define D3DINPUT_H

#include <d3d11.h>
#include <D3DX11.h>
#include <xnamath.h>
#include <dinput.h>
#include <iostream>

const float Pi = 3.1415926535f;

class D3DCamera
{
public:

	D3DCamera():m_Theta(1.5f * Pi), m_Phi(0.25f * Pi), m_Radius(3.0f) 
	{
		m_LastMousePos.x = 0;
		m_LastMousePos.y = 0;

		XMMATRIX I = XMMatrixIdentity();
		m_World =  I;
		m_View  =  I;
		m_Proj  =  I;
	}

	void update();
	void DirectInput(float dt);
	bool InitDirectInput(HWND hWnd,  HINSTANCE hInstance);
    XMMATRIX GetViewMatrix()
	{
		return m_View;
	}

	void DetectInput(HWND hWnd, double time);

private:

	XMMATRIX m_World;
	XMMATRIX m_View;
	XMMATRIX m_Proj;

	float m_Theta;
	float m_Phi;
	float m_Radius;

	POINT m_LastMousePos;
	float moveLeftRight  ;
	float moveBackForward;

	IDirectInputDevice8* m_pDIKeyboard;
	IDirectInputDevice8* m_pDIMouse;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 m_DirectInput;
};



#endif