#ifndef ALPHASHADER_H
#define ALPHASHADER_H

#include <d3d11.h>
#include <xnamath.h>
#include "d3d/Shader.h"
#include "d3d/Utility.h"

namespace byhj
{

	class AlphaShader
	{
	public:
		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix);
		void Shutdown();

	private:
		void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
		void init_texture(ID3D11Device *pD3D11Device);
		void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);

		struct LightBuffer
		{
			XMFLOAT4 ambientColor;
			XMFLOAT4 diffuseColor;
			XMFLOAT3 lightDirection;
			FLOAT    padding;
		};
		LightBuffer cbLight;

		d3d::MatrixBuffer cbMatrix;
		ComPtr<ID3D11InputLayout> m_pInputLayout;
		ComPtr<ID3D11Buffer> m_pMVPBuffer;
		ID3D11Buffer             *m_pLightBuffer      = nullptr;
		ID3D11Buffer             *m_CameraBuffer      = nullptr;
		ID3D11ShaderResourceView *m_pTextures[3];
		ComPtr<ID3D11SamplerState> m_pTexSamplerState;

		d3d::Shader CubeShader;
	};

}
#endif