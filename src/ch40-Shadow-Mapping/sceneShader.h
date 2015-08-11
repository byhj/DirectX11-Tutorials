#include "d3d/d3dShader.h"
#include "d3d/d3dUtility.h"
#include <xnamath.h>

namespace byhj
{


	class SceneShader
	{
	public:
		void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::MatrixBuffer &matrix);
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
			XMFLOAT4X4 lightView;
			XMFLOAT4X4 lightPorj;
		};
		MatrixLight cbMatrix;

		ID3D11Buffer *m_pMVPBuffer;
		ID3D11SamplerState* m_sampleStateWrap;
		ID3D11SamplerState* m_sampleStateClamp;
		ID3D11Buffer* m_pLightBuffer;
		ID3D11Buffer* m_pLightBuffer2;


		byhj::Shader sceneShader;
	};

}