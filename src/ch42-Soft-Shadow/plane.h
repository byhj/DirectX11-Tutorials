#ifndef Plane_H
#define Plane_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "d3d/Shader.h"
#include "d3d/Utility.h"

using namespace DirectX;


namespace byhj
{
	class Plane
	{
	public:
		Plane() = default;
		~Plane() = default;


		void Init(ID3D11Device *pD3D11Device, int width, int height);
		void Render(ID3D11DeviceContext *pD3D11DeviceContext);
		void Update();
		void Shutdown();
		void Clear(ID3D11DeviceContext *pD3D11DeviceContext);

		ID3D11ShaderResourceView** GetSRV()
		{
			return &m_shaderResourceView;
		}

	private:
		void init_buffer (ID3D11Device *pD3D11Device);


		struct  Vertex
		{
			XMFLOAT3 Pos;
			XMFLOAT2 Tex;
		};

		ID3D11Buffer             *m_pVertexBuffer;
		ID3D11Buffer             *m_pIndexBuffer;

		int m_VertexCount;
		int m_IndexCount;

		int m_width;
		int m_height;

		ID3D11Texture2D* m_renderTargetTexture;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11ShaderResourceView* m_shaderResourceView;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilView* m_depthStencilView;
		D3D11_VIEWPORT m_viewport;
	};

}

#endif