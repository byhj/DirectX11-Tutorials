#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"

#include <xnamath.h>

namespace byhj
{


class DepthShader
{
public:
	void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext,  HWND hWnd);
	void Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::MatrixBuffer &matrix); 

	void Shutdown();

private:


	byhj::MatrixBuffer cbMatrix;
	byhj::Shader depthShader;
	ID3D11Buffer *m_pMVPBuffer;
};

}