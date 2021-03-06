#include "d3d/Shader.h"
#include "d3d/Utility.h"

namespace byhj
{


	class SceneShader
	{
	public:
		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::d3d::MatrixBuffer &matrix,
			const XMFLOAT4X4 &LightView1, const XMFLOAT4X4 &LightProj1,
			const XMFLOAT4X4 &LightView2, const XMFLOAT4X4 &LightProj2,
		    const XMFLOAT4 &LightPos1, const XMFLOAT4 &lightPos2 );

		void Shutdown();

	private:
		struct LightBufferType
		{
			XMFLOAT4 ambientColor;
			XMFLOAT4 diffuseColor;
			XMFLOAT4 diffuseColor2;
		};

		struct LightBufferType2
		{
			XMFLOAT4 lightPos1;
			XMFLOAT4 lightPos2;
		};

		struct MatrixLight
		{
			XMFLOAT4X4 model;
			XMFLOAT4X4 view;
			XMFLOAT4X4 proj;
			XMFLOAT4X4 lightView1;
			XMFLOAT4X4 lightPorj1;
		    XMFLOAT4X4 lightView2;
			XMFLOAT4X4 lightProj2;
		};
		MatrixLight cbMatrix;

		ComPtr < ID3D11Buffer      > m_pMVPBuffer;
		ComPtr < ID3D11SamplerState> m_sampleStateWrap;
		ComPtr < ID3D11SamplerState> m_sampleStateClamp;
		ComPtr < ID3D11Buffer	   > m_pLightBuffer;
		ComPtr < ID3D11Buffer	   > m_pLightBuffer2;
	
		d3d::Shader sceneShader;
	};

}