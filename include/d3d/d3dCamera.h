#ifndef D3DCAMERA_H
#define D3DCAMERA_H

#ifdef _WIN32
#define _XM_NO_INTRINSICS_
#endif 

#include <d3d11.h>
#include <xnamath.h>
#include <D3DX10math.h>

const float Pi = 3.1415926535f;

class D3DCamera
{
public:
    D3DCamera():m_Theta(1.5f * Pi), m_Phi(0.25f * Pi), m_Radius(15.0f)
	{
		m_LastMousePos.x = 0;
		m_LastMousePos.y = 0;
		XMMATRIX I = XMMatrixIdentity();
		m_World = I;
		m_View  = I;
		m_Proj  = I;
	}

	void update();
	XMMATRIX GetViewMatrix() const 
	{
		return m_View;
	}
    XMMATRIX GetProjMatrix() const
	{
		return m_Proj;
	}
	XMFLOAT3 GetPos()
	{
		return pos;
	}
	void SetRadius(float r)
	{
		m_Radius = r;
	}
	void OnMouseDown(WPARAM btnState, int x, int y, HWND hWnd);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseWheel(WPARAM btnState, int x, int y, float aspect);

private:

	float m_Theta;
	float m_Phi;
	float m_Radius;
	POINT m_LastMousePos;

	XMMATRIX m_World;
	XMMATRIX m_View;
	XMMATRIX m_Proj;
	XMFLOAT3 pos;
	XMFLOAT3 target;
};



#endif