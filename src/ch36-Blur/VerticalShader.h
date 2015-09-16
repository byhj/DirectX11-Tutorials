#include "d3d/Shader.h"
#include "d3d/Utility.h"

#include <xnamath.h>

namespace byhj
{


	class VerticalShader
	{
	public:
		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::d3d::MatrixBuffer &matrix);

		void Shutdown();

	private:

		byhj::d3d::MatrixBuffer cbMatrix;
		ID3D11Buffer *m_pMVPBuffer;
		d3d::Shader verticalShader;
	};

}