
#include "d3d/d3dShader.h"
#include "d3d/d3dDebug.h"

#include <xnamath.h>

class Bitmap
{
public:
	Bitmap()
	{
		m_pInputLayout        = NULL;
		m_pVS                 = NULL;
		m_pPS                 = NULL;
		m_pRenderTargetView   = NULL;
		m_pMVPBuffer          = NULL;
		m_pVertexBuffer       = NULL;
		m_pIndexBuffer        = NULL;
		m_pTexture            = NULL;
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext);

	void Shutdown()
	{
		    ReleaseCOM(m_pInputLayout       )
			ReleaseCOM(m_pVS                )
			ReleaseCOM(m_pPS                )
			ReleaseCOM(m_pMVPBuffer         )
			ReleaseCOM(m_pRenderTargetView  )
			ReleaseCOM(m_pVertexBuffer      )
			ReleaseCOM(m_pIndexBuffer       )
			ReleaseCOM(m_pTexture           )
	}

public:
	bool init_window(int sw, int sh);
	bool init_buffer (ID3D11Device *pD3D11Device, int posX, int posY);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile);

private:

	struct MatrixBuffer
	{
		XMFLOAT4X4  MVP;
	};
	MatrixBuffer cbMatrix;

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	ID3D11InputLayout        *m_pInputLayout;
	ID3D11VertexShader       *m_pVS;
	ID3D11PixelShader        *m_pPS;
	ID3D11RenderTargetView   *m_pRenderTargetView;
	ID3D11Buffer             *m_pMVPBuffer;
	ID3D11Buffer             *m_pVertexBuffer;
	ID3D11Buffer             *m_pIndexBuffer;
	ID3D11ShaderResourceView *m_pTexture;
	ID3D11SamplerState       *m_pTexSamplerState;

	int VertexCount;
	int IndexCount;

	Shader BitmapShader;

	int screenWidth, screenHeight;
	int bitmapWidth, bitmapHeight;
	int lastPosX, lastPosY;
	float aspect;

	XMMATRIX MVP;
	XMMATRIX Model;
	XMMATRIX View;
	XMMATRIX Proj;

	XMVECTOR camPos;
	XMVECTOR camTarget;
	XMVECTOR camUp;
};

