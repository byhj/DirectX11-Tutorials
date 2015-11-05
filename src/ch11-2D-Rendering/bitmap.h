#ifdef _WIN32
#define _XM_NO_INTRINSICS_
#endif 

#include "d3d/Shader.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "d3d/Utility.h"



namespace byhj
{


class Bitmap
{
public:
	Bitmap();
	~Bitmap();

	void Init(ID3D11Device *pD3D11Device,  HWND hWnd);
	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix);
	void Shutdown();
	bool init_window(int sw, int sh);

public:

	bool init_buffer (ID3D11Device *pD3D11Device, int posX, int posY);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device);

private:

	d3d::MatrixBuffer cbMatrix;

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	ComPtr<ID3D11InputLayout> m_pInputLayout;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_pTexture;
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;

	int VertexCount;
	int IndexCount;

	d3d::Shader BitmapShader;

	int screenWidth, screenHeight;
	int bitmapWidth, bitmapHeight;
	int lastPosX, lastPosY;
	float aspect;
};


}