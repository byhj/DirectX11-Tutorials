#ifndef Frustum_H
#define Frustum_H

#include <d3d11.h>

#ifdef WIN32
#define _XM_NO_INTRINSICS_
#endif

#include <DirectXMath.h>

using namespace DirectX;

namespace byhj
{

namespace d3d
{


class Frustum
{
public:
	Frustum() {}
	Frustum(const Frustum &) {}
	~Frustum() {}

	void Construct(float screenDepth, XMFLOAT4X4 proj , XMFLOAT4X4 view);
	
	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

private:
	XMFLOAT4 m_Planes[6];
};

}

}

#endif