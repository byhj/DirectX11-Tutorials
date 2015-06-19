
#include <common/d3dShader.h>

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
			ReleaseCOM(m_pVertexBuffer      )
			ReleaseCOM(m_pIndexBuffer       )
	}

public:
	bool init_window(int sw, int sh);
	bool init_buffer (ID3D11Device *pD3D11Device, int posX, int posY);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile);

private:

	struct cbPerObject
	{
		XMMATRIX  MVP;
	};
	cbPerObject cbPerObj;

	struct Vertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 Tex;
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



void Bitmap::Render(ID3D11DeviceContext *pD3D11DeviceContext)
{
	Model = XMMatrixIdentity();
	MVP = (Model * View * Proj);
	cbPerObj.MVP = XMMatrixTranspose(MVP);	
	// Set vertex buffer stride and offset.

	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex); 
	offset = 0;
	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	BitmapShader.use(pD3D11DeviceContext);

	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbPerObj, 0, 0 );
	pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, &m_pMVPBuffer);
	pD3D11DeviceContext->PSSetShaderResources( 0, 1, &m_pTexture );
	pD3D11DeviceContext->PSSetSamplers( 0, 1, &m_pTexSamplerState );
	pD3D11DeviceContext->DrawIndexed(IndexCount, 0, 0);

}

bool Bitmap::init_window(int sw, int sh)
{
	screenWidth  = sw;
	screenHeight = sh;
	lastPosX = -1;
	lastPosY = -1;
	bitmapWidth = 256;
	bitmapHeight = 256;

	return true;
}


bool Bitmap::init_buffer(ID3D11Device *pD3D11Device, int posX, int posY)
{
	HRESULT hr;
	float left, right, top, bottom;
	//If the Position we are rendering this bitmap to has not changed then don't update
	if ( (posX == lastPosX) && (posY == lastPosY) )
	{
		return true;
	}

	lastPosX = posX;
	lastPosY = posY;

	// Calculate the screen coordinates of the left side of the bitmap.
	left = (float)((screenWidth / 2) * -1) + (float)posX;

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)bitmapWidth;

	// Calculate the screen coordinates of the top of the bitmap.
	top = (float)(screenHeight / 2) - (float)posY;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)bitmapHeight;

	///////////////////////////Index Buffer ////////////////////////////////
	VertexCount = 6;
	Vertex *VertexData = new Vertex[VertexCount];
	// First triangle.
	VertexData[0].Pos = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	VertexData[0].Tex = D3DXVECTOR2(0.0f, 0.0f);

	VertexData[1].Pos = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	VertexData[1].Tex = D3DXVECTOR2(1.0f, 1.0f);

	VertexData[2].Pos = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	VertexData[2].Tex = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	VertexData[3].Pos = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	VertexData[3].Tex = D3DXVECTOR2(0.0f, 0.0f);

	VertexData[4].Pos = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	VertexData[4].Tex = D3DXVECTOR2(1.0f, 0.0f);

	VertexData[5].Pos = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	VertexData[5].Tex = D3DXVECTOR2(1.0f, 1.0f);


	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth           = sizeof(Vertex) * VertexCount;
	VertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags      = 0;
	VertexBufferDesc.MiscFlags           = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA VBO;
	VBO.pSysMem          = VertexData;
	VBO.SysMemPitch      = 0;
	VBO.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	hr = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
	DebugHR(hr);

	/////////////////////////////////Index Buffer ///////////////////////////////////////
	IndexCount = 6;
	unsigned long *IndexData= new unsigned long[IndexCount];
	IndexData[0] = 0;  // Bottom left.
	IndexData[1] = 1;  // Top middle.
	IndexData[2] = 2;  // Bottom right.
	IndexData[3] = 3;  // Bottom right.
	IndexData[4] = 4;  // Bottom right.
	IndexData[5] = 5;  // Bottom right.

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth           = sizeof(unsigned long) * IndexCount;
	IndexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags      = 0;
	IndexBufferDesc.MiscFlags           = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA IBO;
	IBO.pSysMem          = IndexData;
	IBO.SysMemPitch      = 0;
	IBO.SysMemSlicePitch = 0;

	hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
	DebugHR(hr);

	////////////////////////////////////////////////////////////////////////////////////////
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] VertexData;
	VertexData = 0;
	delete IndexData;
	IndexData = 0;



	////////////////////////////////Const Buffer//////////////////////////////////////

	//MVP Matrix
	camPos    = XMVectorSet( 0.0f, 0.0f, -3.0f, 0.0f );
	camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	camUp     = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	View      = XMMatrixLookAtLH( camPos, camTarget, camUp );
	Proj      = XMMatrixOrthographicLH( (float)screenWidth, (float)screenHeight, 0.1f, 1000.0f);


	D3D11_BUFFER_DESC mvpDesc;	
	ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
	mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpDesc.ByteWidth      = sizeof(XMMATRIX);
	mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpDesc.CPUAccessFlags = 0;
	mvpDesc.MiscFlags      = 0;
	pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);

	return true;
}


bool Bitmap::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc[2];
	pInputLayoutDesc[0].SemanticName         = "POSITION";
	pInputLayoutDesc[0].SemanticIndex        = 0;
	pInputLayoutDesc[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[0].InputSlot            = 0;
	pInputLayoutDesc[0].AlignedByteOffset    = 0;
	pInputLayoutDesc[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[0].InstanceDataStepRate = 0;

	pInputLayoutDesc[1].SemanticName         = "TEXCOORD";
	pInputLayoutDesc[1].SemanticIndex        = 0;
	pInputLayoutDesc[1].Format               = DXGI_FORMAT_R32G32_FLOAT;
	pInputLayoutDesc[1].InputSlot            = 0;
	pInputLayoutDesc[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[1].InstanceDataStepRate = 0;
	unsigned numElements = ARRAYSIZE(pInputLayoutDesc);

	BitmapShader.init(pD3D11Device, hWnd);
	BitmapShader.attachVS(L"bitmap.vsh", pInputLayoutDesc, numElements);
	BitmapShader.attachPS(L"bitmap.psh");
	BitmapShader.end();

	return true;
}

void Bitmap::init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile)
{
	HRESULT hr;
	hr = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, texFile, NULL,NULL, &m_pTexture, NULL);
	DebugHR(hr);

	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = pD3D11Device->CreateSamplerState(&samplerDesc, &m_pTexSamplerState);
	DebugHR(hr);

}
