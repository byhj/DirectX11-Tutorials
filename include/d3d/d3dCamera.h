#ifndef D3DCAMERA_H
#define D3DCAMERA_H

#ifdef _WIN32
#define _XM_NO_INTRINSICS_
#endif 

#include <d3d11.h>
#include <xnamath.h>
#include <D3DX10math.h>
#include <vector>

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
		pickFlag = false;
	}

	void init(int sw, int sh, std::vector<XMFLOAT3> vPos, std::vector<unsigned long> vIndex, XMMATRIX pickModel)
	{
		ClientWidth = sw;
		ClientHeight = sh;
		m_aspect = float(sw) / sh;
		vertPosArray = vPos;
		indexPosArray = vIndex;
		this->pickModel = pickModel;
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
	bool CheckPick()
	{
		return pickFlag;
	}
	void SetRadius(float r)
	{
		m_Radius = r;
	}
	void OnMouseDown(WPARAM btnState, int x, int y, HWND hWnd);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseWheel(WPARAM btnState, int x, int y, float aspect);

	bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point );

	float pick(XMVECTOR pickRayInWorldSpacePos, XMVECTOR pickRayInWorldSpaceDir, 
		std::vector<XMFLOAT3>& vertPosArray, std::vector<DWORD>& indexPosArray, 
		XMMATRIX& worldSpace);
	void pickRayVector(float mouseX, float mouseY, XMVECTOR& pickRayInWorldSpacePos, XMVECTOR& pickRayInWorldSpaceDir);

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
	float m_aspect;

	std::vector<XMFLOAT3> vertPosArray;
	std::vector<unsigned long> indexPosArray;
	int ClientWidth; 
	int ClientHeight;
	XMMATRIX pickModel;
	bool pickFlag;
};



#endif