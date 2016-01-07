#ifndef WaterShader_H
#define WaterShader_H

#include "d3d/shader.h"
#include "d3d/Utility.h"

namespace byhj
{
	class WaterShader
	{
	public:
		WaterShader() = default;
		~WaterShader() = default;

		struct ReflectMatrixBuffer
		{
			XMFLOAT4X4  model;
			XMFLOAT4X4  view;
			XMFLOAT4X4  proj;
			XMFLOAT4X4  reflectMat;

		};
		ReflectMatrixBuffer cbMatrix;

		struct WaterBufer
		{
			FLOAT waterTranslation;
			FLOAT reflectRefractScale;
			XMFLOAT2 padding;
		};
		WaterBufer water;

		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Update();
		void Use(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix, const XMFLOAT4X4 &relfectMat);
		void Shutdown();

	private:
		d3d::Shader m_WaterShader;
		d3d::MatrixBuffer m_cbMatrix;

		ComPtr<ID3D11Buffer> m_pMVPBuffer;
		ComPtr<ID3D11Buffer> m_pWaterBuffer;
		ComPtr<ID3D11ShaderResourceView> m_pTexture;
		ComPtr<ID3D11SamplerState> m_pTexSamplerState;
		ComPtr<ID3D11InputLayout> m_pInputLayout;
	};
}
#endif
