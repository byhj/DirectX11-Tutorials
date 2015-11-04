  #ifndef D3DUTILITY_H
#define D3DUTILITY_H

#include <windows.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace byhj
{

namespace d3d
{

struct MatrixBuffer
{
	XMFLOAT4X4 model;
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
};

}

}


#endif