#include "d3d/Shader.h"
#include "d3d/Utility.h"

#include <xnamath.h>

namespace byhj
{


	class HorizontalShader
	{
	public:
		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::MatrixBuffer &matrix);

		void Shutdown();

	private:


		byhj::MatrixBuffer cbMatrix;
		ID3D11Buffer *m_pMVPBuffer;
		d3d::Shader horizontalShader;
	};

}