#ifndef LightShader_H
#define LightShader_H

#include "d3d/Shader.h"
#include "d3d/Utility.h"

namespace byhj
{
	class LightShader
	{
	public:
		LightShader() = default;
		~LightShader() = default;

		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Update();
		void Use(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix);
		void Shutdown();

		struct CameraBuffer
		{
			XMFLOAT3 camPos;
			float padding;
		};

		d3d::MatrixBuffer cbMatrix;

		struct LightBuffer
		{
			XMFLOAT4 ambientColor;
			XMFLOAT4 diffuseColor;
			XMFLOAT3 lightDirection;
			FLOAT  padding;
		};
		LightBuffer cbLight;

	private:

		d3d::Shader m_LightShader;

		ComPtr<ID3D11Buffer> m_pMVPBuffer;
		ComPtr<ID3D11Buffer> m_pLightBuffer;
		ComPtr<ID3D11Buffer> m_CameraBuffer;
		ComPtr<ID3D11SamplerState> m_pTexSamplerState;
		ComPtr<ID3D11InputLayout> m_pInputLayout;
	};
}
#endif