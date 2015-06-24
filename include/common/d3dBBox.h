#ifndef D3DBBOX_H
#define D3DBBOX_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

BoundingBox box;

class D3DBBox
{
public:
	D3DBBox() {}

	BoundingBox bbox;
	XMFLOAT3 Center;            // Center of the box.
	XMFLOAT3 Extents;           // Distance from the center to each side.
};

#endif