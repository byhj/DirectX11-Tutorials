#include "d3d/Shader.h"
#include "d3d/Utility.h"



namespace byhj
{


	class PlaneShader
	{
	public:
		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::d3d::MatrixBuffer &matrix);
		void Shutdown();

	private:


		d3d::MatrixBuffer cbMatrix;
		d3d::Shader planeShader;
		ID3D11Buffer *m_pMVPBuffer;
	};

}