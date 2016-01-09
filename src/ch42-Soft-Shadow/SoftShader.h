#include "d3d/Shader.h"
#include "d3d/Utility.h"

namespace byhj
{


	class SoftShader
	{
	public:
		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::d3d::MatrixBuffer &matrix, const XMFLOAT4 &LightPos);
		void Shutdown();

	private:
		struct LightBufferType
		{
			XMFLOAT4 ambientColor;
			XMFLOAT4 diffuseColor;
		};

		struct LightBufferType2
		{
			XMFLOAT3 lightPosition;
			float padding;
		};

		struct MatrixLight
		{
			XMFLOAT4X4 model;
			XMFLOAT4X4 view;
			XMFLOAT4X4 proj;
		};
		MatrixLight cbMatrix;

		ComPtr < ID3D11Buffer      > m_pMVPBuffer;
		ComPtr < ID3D11SamplerState> m_sampleStateWrap;
		ComPtr < ID3D11SamplerState> m_sampleStateClamp;
		ComPtr < ID3D11Buffer	   > m_pLightBuffer;
		ComPtr < ID3D11Buffer	   > m_pLightBuffer2;
	
		d3d::Shader softShader;
	};

}