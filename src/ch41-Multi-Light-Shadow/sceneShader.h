#include "d3d/Shader.h"
#include "d3d/Utility.h"


namespace byhj
{


	class SceneShader
	{
	public:
		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::d3d::MatrixBuffer &matrix, const XMFLOAT4X4 &LightView, const XMFLOAT4X4 &LightProj,
			      const XMFLOAT4X4 &LightView2, const XMFLOAT4X4 &LightProj2);
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
			XMFLOAT3 lightPos;
			float padding;
			XMFLOAT3 lightPos2;
			float padding2;
		};
		LightBufferType2 lightPosBuffer;

		struct MatrixLight
		{
			XMFLOAT4X4 model;
			XMFLOAT4X4 view;
			XMFLOAT4X4 proj;
			XMFLOAT4X4 lightView;
			XMFLOAT4X4 lightPorj;
			XMFLOAT4X4 lightView2;
			XMFLOAT4X4 lightProj2;
		};
		MatrixLight cbMatrix;

		ID3D11Buffer *m_pMVPBuffer;
		ID3D11SamplerState* m_sampleStateWrap;
		ID3D11SamplerState* m_sampleStateClamp;
		ID3D11Buffer* pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, m_pLightBuffer.GetAddressOf());;
		ID3D11Buffer* pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, m_pLightBuffer.GetAddressOf());2;


		d3d::Shader sceneShader;
	};

}