#ifndef GlowShader_H
#define GlowShader_H


#include "d3d/Shader.h"
#include "d3d/Utility.h"

namespace byhj
{


	class GlowShader
	{
	public:
		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::d3d::MatrixBuffer &matrix, 
			      ID3D11ShaderResourceView *pTexture1, ID3D11ShaderResourceView *pTexture2);
		void Shutdown();

	private:

		d3d::MatrixBuffer cbMatrix;
		d3d::Shader glowShader;
		ComPtr<ID3D11Buffer> m_pMVPBuffer;
		ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	};

}

#endif