#ifndef D3DFRUSTUM_H
#define D3DFRUSTUM_H

#include <D3DX11.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <xnamath.h>

class D3DFrustum
{
public:
	D3DFrustum() {}
	D3DFrustum(const D3DFrustum &) {}
	~D3DFrustum() {}

	void Construct(float screenDepth, D3DXMATRIX proj , D3DXMATRIX view);
	
	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

private:
	D3DXPLANE m_Planes[6];
};

void D3DFrustum::Construct(float screenDepth, D3DXMATRIX proj , D3DXMATRIX view)
{
	float zMinimum, r;
	D3DXMATRIX matrix;

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -proj._43 / proj._33;
	r = screenDepth / (screenDepth - zMinimum);
	proj._33 = r;
	proj._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	D3DXMatrixMultiply(&matrix, &view, &proj);

	// Calculate near plane of frustum.
	m_Planes[0].a = matrix._14 + matrix._13;
	m_Planes[0].b = matrix._24 + matrix._23;
	m_Planes[0].c = matrix._34 + matrix._33;
	m_Planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_Planes[0], &m_Planes[0]);

	// Calculate far plane of frustum.
	m_Planes[1].a = matrix._14 - matrix._13; 
	m_Planes[1].b = matrix._24 - matrix._23;
	m_Planes[1].c = matrix._34 - matrix._33;
	m_Planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_Planes[1], &m_Planes[1]);

	// Calculate left plane of frustum.
	m_Planes[2].a = matrix._14 + matrix._11; 
	m_Planes[2].b = matrix._24 + matrix._21;
	m_Planes[2].c = matrix._34 + matrix._31;
	m_Planes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&m_Planes[2], &m_Planes[2]);

	// Calculate right plane of frustum.
	m_Planes[3].a = matrix._14 - matrix._11; 
	m_Planes[3].b = matrix._24 - matrix._21;
	m_Planes[3].c = matrix._34 - matrix._31;
	m_Planes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&m_Planes[3], &m_Planes[3]);

	// Calculate top plane of frustum.
	m_Planes[4].a = matrix._14 - matrix._12; 
	m_Planes[4].b = matrix._24 - matrix._22;
	m_Planes[4].c = matrix._34 - matrix._32;
	m_Planes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&m_Planes[4], &m_Planes[4]);

	// Calculate bottom plane of frustum.
	m_Planes[5].a = matrix._14 + matrix._12;
	m_Planes[5].b = matrix._24 + matrix._22;
	m_Planes[5].c = matrix._34 + matrix._32;
	m_Planes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&m_Planes[5], &m_Planes[5]);
}

bool D3DFrustum::CheckPoint(float x, float y, float z)
{
	// Check if the point is inside all six planes of the view frustum.
	for(int i=0; i<6; i++) 
	{
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(x, y, z)) < 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool D3DFrustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
   // Check if any one point of the cube is in the view frustum.
	for(int i=0; i<6; i++) 
	{
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter + radius))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool D3DFrustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	// Check if the radius of the sphere is inside the view frustum.
	for(int i=0; i<6; i++) 
	{
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3(xCenter, yCenter, zCenter)) < -radius)
		{
			return false;
		}
	}

	return true;
}

bool D3DFrustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	// Check if any of the 6 planes of the rectangle are inside the view frustum.
	for(int i=0; i<6; i++)
	{
		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_Planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

#endif