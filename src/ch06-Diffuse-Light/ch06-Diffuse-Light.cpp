#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

#ifdef _WIN32
#define _XM_NO_INTRINSICS_
#endif 

#include "d3d/d3dApp.h"
#include <d3d/d3dShader.h>

class D3DInitApp: public D3DApp
{
public:
	D3DInitApp()
	{
		m_AppName = L"DirectX11: ch04-Buffer-Shader";

		m_pInputLayout        = NULL;
		m_pVS                 = NULL;
		m_pPS                 = NULL;
		m_pSwapChain          = NULL;
		m_pD3D11Device        = NULL;
		m_pD3D11DeviceContext = NULL;
		m_pRenderTargetView   = NULL;
		m_pDepthStencilView   = NULL;
		m_pDepthStencilBuffer = NULL;
		m_pMVPBuffer          = NULL;
		m_pLightBuffer        = NULL;
		m_pVertexBuffer       = NULL;
		m_pIndexBuffer        = NULL;
		m_pTexture            = NULL;
	}

	bool v_InitD3D();
	void v_Render();

    void v_Shutdown()
	{
		ReleaseCOM(m_pInputLayout       )
		ReleaseCOM(m_pVS                )
		ReleaseCOM(m_pPS                )
		ReleaseCOM(m_pSwapChain         )
		ReleaseCOM(m_pD3D11Device       )
		ReleaseCOM(m_pD3D11DeviceContext)
		ReleaseCOM(m_pRenderTargetView  )
		ReleaseCOM(m_pDepthStencilView  )
		ReleaseCOM(m_pDepthStencilBuffer)
		ReleaseCOM(m_pMVPBuffer         )
		ReleaseCOM(m_pLightBuffer       )
		ReleaseCOM(m_pVertexBuffer      )
		ReleaseCOM(m_pIndexBuffer       )
	}

private:
	bool init_buffer();
	bool init_device();
	bool init_shader();
	bool init_camera();
	void init_texture();

private:

	struct MatrixBuffer
	{
		XMFLOAT4X4  model;
		XMFLOAT4X4  view;
		XMFLOAT4X4  proj;

	};
	MatrixBuffer cbMatrix;

	struct LightBuffer
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};
	LightBuffer cbLight;

	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
		XMFLOAT3 Normal;
	};

	ID3D11InputLayout       *m_pInputLayout;
	ID3D11VertexShader      *m_pVS;
	ID3D11PixelShader       *m_pPS;
	IDXGISwapChain          *m_pSwapChain;
	ID3D11Device            *m_pD3D11Device;
	ID3D11DeviceContext     *m_pD3D11DeviceContext;
	ID3D11RenderTargetView  *m_pRenderTargetView;
	ID3D11DepthStencilView  *m_pDepthStencilView;
	ID3D11Texture2D         *m_pDepthStencilBuffer;
	ID3D11Buffer            *m_pMVPBuffer;
	ID3D11Buffer            *m_pLightBuffer;
	ID3D11Buffer            *m_pVertexBuffer;
	ID3D11Buffer            *m_pIndexBuffer;
	ID3D11ShaderResourceView *m_pTexture;
	ID3D11SamplerState       *m_pTexSamplerState;

	int m_VertexCount;
	int m_IndexCount;

	Shader TestShader;

	XMMATRIX Model;
	XMMATRIX View;
	XMMATRIX Proj;

	XMVECTOR camPos;
	XMVECTOR camTarget;
	XMVECTOR camUp;
};

CALL_MAIN(D3DInitApp);

bool D3DInitApp::v_InitD3D()
{
    init_device();
	init_camera();
	init_buffer();
	init_texture();
	init_shader();

 	return true;
}

void D3DInitApp::v_Render()
{

	TestShader.use(m_pD3D11DeviceContext);
	//Render scene 
	//Keep the cubes rotating
	static float rot = 0.0f;
	rot += .0001f;

	//Update the mvp matrix
	float bgColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMVECTOR rotaxis = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	         Model   = XMMatrixRotationAxis( rotaxis, rot); 

	XMMATRIX tempModel =  XMMatrixTranspose(Model);
	XMMATRIX tempView  =  XMMatrixTranspose(View);
	XMMATRIX tempProj  =  XMMatrixTranspose(Proj);
	XMStoreFloat4x4(&cbMatrix.model, tempModel);
	XMStoreFloat4x4(&cbMatrix.view,  tempView);
	XMStoreFloat4x4(&cbMatrix.proj,  tempProj);

	m_pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbMatrix, 0, 0 );
	m_pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, &m_pMVPBuffer);

	m_pD3D11DeviceContext->PSSetShaderResources( 0, 1, &m_pTexture );
	m_pD3D11DeviceContext->PSSetSamplers( 0, 1, &m_pTexSamplerState );
	m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView, bgColor);
	m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
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
	DebugHR(hr);
	hr = m_pD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	DebugHR(hr);
	pBackBuffer->Release();
	m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);


	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width              = m_ScreenWidth;
	depthStencilDesc.Height             = m_ScreenHeight;
	depthStencilDesc.MipLevels          = 1;
	depthStencilDesc.ArraySize          = 1;
	depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags     = 0; 
	depthStencilDesc.MiscFlags          = 0;

	m_pD3D11Device->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthStencilBuffer);
	m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView);
	m_pD3D11DeviceContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

	return true;
}

bool D3DInitApp::init_buffer()
{
	HRESULT hr;

	///////////////////////////Index Buffer ////////////////////////////////
	m_VertexCount = 3;
	Vertex *VertexData = new Vertex[m_VertexCount];

	VertexData[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	VertexData[1].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);    // Top middle.
	VertexData[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);   // Bottom right.

	VertexData[0].TexCoord = XMFLOAT2(0.0f, 1.0f);
	VertexData[1].TexCoord = XMFLOAT2(0.5f, 0.0f);
	VertexData[2].TexCoord = XMFLOAT2(1.0f, 1.0f);

	VertexData[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	VertexData[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	VertexData[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

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
	hr = m_pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
    DebugHR(hr);

	/////////////////////////////////Index Buffer ///////////////////////////////////////
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

	hr = m_pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
    DebugHR(hr);

	////////////////////////////////////////////////////////////////////////////////////////
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] VertexData;
	VertexData = 0;
	delete [] IndexData;
	IndexData = 0;

	// Set vertex buffer stride and offset.=
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex); 
	offset = 0;
	m_pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	////////////////////////////////MVP Buffer//////////////////////////////////////

	D3D11_BUFFER_DESC mvpBufferDesc;	
	ZeroMemory(&mvpBufferDesc, sizeof(D3D11_BUFFER_DESC));
	mvpBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpBufferDesc.ByteWidth      = sizeof(MatrixBuffer);
	mvpBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpBufferDesc.CPUAccessFlags = 0;
	mvpBufferDesc.MiscFlags      = 0;
	hr = m_pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);
	DebugHR(hr);
/*
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// Lock the light constant buffer so it can be written to.
	hr = m_pD3D11DeviceContext->Map(m_pMVPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	DebugHR(hr);

	// Get a pointer to the data in the constant buffer.
	MatrixBuffer *dataPtr1 = (MatrixBuffer*)mappedResource.pData;

	dataPtr1->model = XMMatrixTranspose(Model);// XMMatrixTranspose(Model);
	dataPtr1->view  = XMMatrixTranspose(View );
	dataPtr1->proj  = XMMatrixTranspose(Proj );

	m_pD3D11DeviceContext->Unmap(m_pMVPBuffer, 0);
	m_pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, &m_pMVPBuffer);
	*/
	///////////////////////////////////////Light buffer////////////////////////////////////////
	D3D11_BUFFER_DESC lightBufferDesc;	
	ZeroMemory(&lightBufferDesc, sizeof(D3D11_BUFFER_DESC));
	lightBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth      = sizeof(LightBuffer);
	lightBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags      = 0;

	hr = m_pD3D11Device->CreateBuffer(&lightBufferDesc, NULL, &m_pLightBuffer);
	DebugHR(hr);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// Lock the light constant buffer so it can be written to.
	hr = m_pD3D11DeviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    DebugHR(hr);

	// Get a pointer to the data in the constant buffer.
	LightBuffer *dataPtr2 = (LightBuffer*)mappedResource.pData;

	dataPtr2->diffuseColor   = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	dataPtr2->lightDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	dataPtr2->padding = 0.0f;

	m_pD3D11DeviceContext->Unmap(m_pLightBuffer, 0);

	int lightSlot = 0;
	m_pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, &m_pLightBuffer);

	return true;
}

bool D3DInitApp::init_camera()
{
	//Viewport Infomation
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = m_ScreenWidth;
	vp.Height   = m_ScreenHeight;
	m_pD3D11DeviceContext->RSSetViewports(1, &vp);

	//MVP Matrix
	camPos    = XMVectorSet( 0.0f, 0.0f, -3.0f, 0.0f );
	camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	camUp     = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	View      = XMMatrixLookAtLH( camPos, camTarget, camUp );
	Proj      = XMMatrixPerspectiveFovLH( 0.4f*3.14f, GetAspect(), 1.0f, 1000.0f);
	Model     = XMMatrixIdentity();

	return true;
}

bool D3DInitApp::init_shader()
{
	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc[3];
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

	pInputLayoutDesc[2].SemanticName         = "NORMAL";
	pInputLayoutDesc[2].SemanticIndex        = 0;
	pInputLayoutDesc[2].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[2].InputSlot            = 0;
	pInputLayoutDesc[2].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[2].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[2].InstanceDataStepRate = 0;

	unsigned numElements = ARRAYSIZE(pInputLayoutDesc);

	TestShader.init(m_pD3D11Device, GetHwnd());
	TestShader.attachVS(L"triangle.vsh", pInputLayoutDesc, numElements);
	TestShader.attachPS(L"triangle.psh");
	TestShader.end();

	return true;
}

void D3DInitApp::init_texture()
{
	HRESULT hr;
	hr = D3DX11CreateShaderResourceViewFromFile(m_pD3D11Device, L"../../media/textures/seafloor.dds", NULL,NULL, &m_pTexture, NULL);
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
	hr = m_pD3D11Device->CreateSamplerState(&samplerDesc, &m_pTexSamplerState);
	DebugHR(hr);

}
