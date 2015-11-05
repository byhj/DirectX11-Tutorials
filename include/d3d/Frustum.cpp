#include "Frustum.h"
#include <DirectXCollision.h>

namespace byhj
{

namespace d3d
{

void Frustum::Construct(float screenDepth, XMFLOAT4X4 proj , XMFLOAT4X4 view)
{
	float zMinimum, r;
	XMMATRIX matrix;

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -proj._43 / proj._33;
	r = screenDepth / (screenDepth - zMinimum);
	proj._33 = r;
	proj._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	matrix = XMLoadFloat4x4(&view) * XMLoadFloat4x4(&proj);


	// Calculate near plane of frustum.
	m_Planes[0].x = matrix._14 + matrix._13;
	m_Planes[0].y = matrix._24 + matrix._23;
	m_Planes[0].z = matrix._34 + matrix._33;
	m_Planes[0].w = matrix._44 + matrix._43;
	XMStoreFloat4(&m_Planes[0], XMVector4Normalize( XMLoadFloat4(&m_Planes[0])) );

	// Calculate far plane of frustum.
	m_Planes[1].x = matrix._14 - matrix._13;
	m_Planes[1].y = matrix._24 - matrix._23;
	m_Planes[1].z = matrix._34 - matrix._33;
	m_Planes[1].w = matrix._44 - matrix._43;
	XMStoreFloat4(&m_Planes[1], XMVector4Normalize(XMLoadFloat4(&m_Planes[1])));

	// Calculate left plane of frustum.
	m_Planes[2].x = matrix._14 + matrix._11;
	m_Planes[2].y = matrix._24 + matrix._21;
	m_Planes[2].z = matrix._34 + matrix._31;
	m_Planes[2].w = matrix._44 + matrix._41;
	XMStoreFloat4(&m_Planes[2], XMVector4Normalize(XMLoadFloat4(&m_Planes[2])));

	// Calculate right plane of frustum.
	m_Planes[3].x = matrix._14 - matrix._11;
	m_Planes[3].y = matrix._24 - matrix._21;
	m_Planes[3].z = matrix._34 - matrix._31;
	m_Planes[3].w = matrix._44 - matrix._41;
	XMStoreFloat4(&m_Planes[3], XMVector4Normalize(XMLoadFloat4(&m_Planes[3])));

	// Calculate top plane of frustum.
	m_Planes[4].x = matrix._14 - matrix._12;
	m_Planes[4].y = matrix._24 - matrix._22;
	m_Planes[4].z = matrix._34 - matrix._32;
	m_Planes[4].w = matrix._44 - matrix._42;
	XMStoreFloat4(&m_Planes[4], XMVector4Normalize(XMLoadFloat4(&m_Planes[4])));

	// Calculate bottom plane of frustum.
	m_Planes[5].x = matrix._14 + matrix._12;
	m_Planes[5].y = matrix._24 + matrix._22;
	m_Planes[5].z = matrix._34 + matrix._32;
	m_Planes[5].w = matrix._44 + matrix._42;
	XMStoreFloat4(&m_Planes[5], XMVector4Normalize(XMLoadFloat4(&m_Planes[5])));
}

bool Frustum::CheckPoint(float x, float y, float z)
{
	// Check if the point is inside all six planes of the view frustum.
	for(int i=0; i<6; i++) 
	{
		auto dot = XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet(x, y, z, 0.0f));

		if( XMVectorGetX(dot) < 0.0f)
			return false;
	}

	return true;
}

bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	// Check if any one point of the cube is in the view frustum.
	for(int i=0; i<6; i++) 
	{
		auto dot = XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter - radius), 0.0f));

		if(XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}

		dot = XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter - radius), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}

		dot = XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter - radius), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}

		dot = XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter - radius), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}

		dot = XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter + radius), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}
		dot = XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter + radius), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}

		dot = XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter + radius), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}


		dot = XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter + radius), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	// Check if the radius of the sphere is inside the view frustum.
	for(int i=0; i<6; i++) 
	{
		auto dot = XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet(xCenter, yCenter, zCenter, 0.0f));
		if(XMVectorGetX(dot) < -radius)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	// Check if any of the 6 planes of the rectangle are inside the view frustum.
	for(int i=0; i<6; i++)
	{
		auto dot =XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 0.0f));

		if(XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}

		dot =XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}


		dot =XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}


		dot =XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}


		dot =XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}


		dot =XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}


		dot =XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}


		dot =XMPlaneDotCoord(XMLoadFloat4(&m_Planes[i]), XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 0.0f));

		if (XMVectorGetX(dot) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}	

}
}