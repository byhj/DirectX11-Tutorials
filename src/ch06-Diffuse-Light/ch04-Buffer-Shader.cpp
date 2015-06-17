#include "common/d3dApp.h"
#include <common/d3dShader.h>







class D3DInitApp: public D3DApp
{
public:
	D3DInitApp()
	{
		m_AppName = L"DirectX11: ch04-Buffer-Shader";
		m_pSwapChain          = 0;
		m_pD3D11Device        = 0;
		m_pD3D11DeviceContext = 0;
		m_pRenderTargetView   = 0;
	}

	bool v_InitD3D();
	void v_Render();
	XMMATRIX MVP;
	XMMATRIX Model;
	XMMATRIX View;
	XMMATRIX Proj;
private:
	bool init_buffer();
	bool init_device();
	bool init_shader();


	struct cbPerObject
	{
		XMMATRIX  MVP;
	};
	cbPerObject cbPerObj;

	struct  Vertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR4 Color;
	};
	XMVECTOR camPos;
	XMVECTOR camTarget;
	XMVECTOR camUp;
	ID3D11Buffer            *m_pMVPBuffer;
	ID3D11InputLayout       *m_pInputLayout;
	ID3D11VertexShader      *m_pVS;
	ID3D11PixelShader       *m_pPS;
	IDXGISwapChain          *m_pSwapChain;
	ID3D11Device            *m_pD3D11Device;
	ID3D11DeviceContext     *m_pD3D11DeviceContext;
	ID3D11RenderTargetView  *m_pRenderTargetView;

	ID3D11Buffer            *m_pVertexBuffer;
	ID3D11Buffer            *m_pIndexBuffer;
	int m_VertexCount;
	int m_IndexCount;
	Shader TestShader;
};

CALL_MAIN(D3DInitApp);

bool D3DInitApp::v_InitD3D()
{
	init_device();
	init_buffer();
	init_shader();


 	return true;
}

void D3DInitApp::v_Render()
{
	//Render 
	D3DXCOLOR bgColor( 0.0f, 0.0f, 0.0f, 1.0f );
	Model = XMMatrixIdentity();
	MVP = (Model * View * Proj);
	cbPerObj.MVP = XMMatrixTranspose(MVP);	
	m_pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbPerObj, 0, 0 );
	m_pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, &m_pMVPBuffer);
	m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView, bgColor);
	m_pD3D11DeviceContext->DrawIndexed(3, 0, 0);

	m_pSwapChain->Present(0, 0);
}

bool D3DInitApp::init_device()
{
	HRESULT hr;

	//Create buffer desc
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width                   = m_ScreenWidth;
	bufferDesc.Height                  = m_ScreenHeight;
	bufferDesc.RefreshRate.Numerator   = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;

	//Create swapChain Desc
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc         = bufferDesc;
	swapChainDesc.SampleDesc.Count   = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount        = 1;
	swapChainDesc.OutputWindow       = GetHwnd();
	swapChainDesc.Windowed           = TRUE;
	swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;

	//Create the double buffer chain
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, 
		&swapChainDesc, &m_pSwapChain, &m_pD3D11Device,
		NULL, &m_pD3D11DeviceContext);

	//Create backbuffer, buffer also is a texture
	ID3D11Texture2D *pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	hr = m_pD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();

	m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	return true;
}

bool D3DInitApp::init_buffer()
{
	HRESULT result;



	return true;
}

bool D3DInitApp::init_shader()
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

	pInputLayoutDesc[1].SemanticName         = "COLOR";
	pInputLayoutDesc[1].SemanticIndex        = 0;
	pInputLayoutDesc[1].Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pInputLayoutDesc[1].InputSlot            = 0;
	pInputLayoutDesc[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[1].InstanceDataStepRate = 0;
	UINT numElements = ARRAYSIZE(pInputLayoutDesc);

	TestShader.init(m_pD3D11Device, GetHwnd());
	TestShader.attachVS(L"triangle.vsh", pInputLayoutDesc);
	TestShader.attachPS(L"triangle.psh");
	TestShader.use();

	m_pD3D11DeviceContext->IASetInputLayout(TestShader.GetInputLayout());
	m_pD3D11DeviceContext->VSSetShader(TestShader.GetVS(), NULL, 0);
	m_pD3D11DeviceContext->PSSetShader(TestShader.GetPS(), NULL, 0);


	/////////////////////////Index Buffer ///////////////////////////
	m_VertexCount = 3;
	Vertex *VertexData = new Vertex[m_VertexCount];

	VertexData[0].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	VertexData[0].Color    = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	VertexData[1].Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	VertexData[1].Color    = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	VertexData[2].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	VertexData[2].Color    = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth           = sizeof(Vertex) * m_VertexCount;
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
	result = m_pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	/////////////////////////Index Buffer ///////////////////////////
	m_IndexCount = 3;
	unsigned long *IndexData= new unsigned long[m_IndexCount];
	IndexData[0] = 0;  // Bottom left.
	IndexData[1] = 1;  // Top middle.
	IndexData[2] = 2;  // Bottom right.

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_IndexCount;
	IndexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags      = 0;
	IndexBufferDesc.MiscFlags           = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA IBO;
	IBO.pSysMem          = IndexData;
	IBO.SysMemPitch      = 0;
	IBO.SysMemSlicePitch = 0;

	result = m_pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] VertexData;
	VertexData = 0;

	delete [] IndexData;
	IndexData = 0;


	unsigned int stride;
	unsigned int offset;
	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex); 
	offset = 0;
	m_pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//set viewpot
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = m_ScreenWidth;
	vp.Height   = m_ScreenHeight;
	m_pD3D11DeviceContext->RSSetViewports(1, &vp);


	D3D11_BUFFER_DESC mvpDesc;	
	ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
	mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpDesc.ByteWidth      = sizeof(XMMATRIX);
	mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpDesc.CPUAccessFlags = 0;
	mvpDesc.MiscFlags      = 0;
	m_pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);

	camPos    = XMVectorSet( 0.0f, 0.0f, -5.0f, 0.0f );
	camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	camUp     = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	View      = XMMatrixLookAtLH( camPos, camTarget, camUp );
	Proj      = XMMatrixPerspectiveFovLH( 0.4f*3.14f, GetAspect(), 1.0f, 1000.0f);

	return true;
}