#include "common/d3dApp.h"
#include <common/d3dShader.h>


const int Width = 800;
const int Height = 800;

struct cbPerObject
{
	XMMATRIX  MVP;
};
cbPerObject cbPerObj;
struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, float r, float g, float b, float a)
		:Pos(x, y, z), Color(r, g, b, a) {}

	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};


class D3DInitApp: public D3DApp
{
public:
	D3DInitApp()
	{
		m_AppName = L"DirectX11: ch04-Buffer-Shader";
	}

	bool v_InitD3D();
	void v_Render();

private:
	bool init_buffer();
	bool init_device();
	bool init_shader();
	IDXGISwapChain         *pSwapChain ;
	ID3D11Device           *pDevice ;
	ID3D11DeviceContext    *pDeviceContext ;
	ID3D11RenderTargetView *pRenderTargetView ;
	ID3D11DepthStencilView *pDepthStencilView;
	ID3D11Texture2D        *pDepthStencilBuffer;
	ID3D11Buffer           * pMVPBuffer;

	ID3D11Buffer           *pVB;
	ID3D11Buffer           *pIB;
	ID3D11VertexShader     *pVS;
	ID3D11PixelShader      *pPS;
	ID3D10Blob             *VS_Buffer;
	ID3D10Blob             *PS_Buffer;
	ID3D11InputLayout      *pInputLayout;

	XMMATRIX MVP;
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
		init_buffer();
	init_shader();


	return true;
}

void D3DInitApp::v_Render()
{
	D3DXCOLOR bgColor( 0.0, 0.0, 0.0, 1.0f );
	//Set the World/View/Projection matrix, then send it to constant buffer in effect file
	Model = XMMatrixIdentity();
	MVP = Model * View * Proj;

	cbPerObj.MVP = XMMatrixTranspose(MVP);	
	pDeviceContext->UpdateSubresource(pMVPBuffer, 0, NULL, &cbPerObj, 0, 0 );
	pDeviceContext->VSSetConstantBuffers( 0, 1, &pMVPBuffer);
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, bgColor);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	pDeviceContext->DrawIndexed(6, 0, 0);

	pSwapChain->Present(0, 0);
}

bool D3DInitApp::init_device()
{
	HRESULT hr;
	//create buffer desc
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width                   = Width;
	bufferDesc.Height                  = Height;
	bufferDesc.RefreshRate.Numerator   = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;

	//create swapChain Desc
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc         = bufferDesc;
	swapChainDesc.SampleDesc.Count   = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount        = 1;
	swapChainDesc.OutputWindow       =GetHwnd();
	swapChainDesc.Windowed           = TRUE;
	swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, 
		&swapChainDesc, &pSwapChain, &pDevice,
		NULL, &pDeviceContext);

	//create backbuffer
	ID3D11Texture2D *backBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	hr = pDevice->CreateRenderTargetView(backBuffer, NULL, &pRenderTargetView);
	backBuffer->Release();

	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	return true;
}

bool D3DInitApp::init_buffer()
{
	HRESULT result;
	//vs, ps shader input layout
	HRESULT hr;
	hr = D3DX11CompileFromFile(L"triangle.vsh", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile(L"triangle.psh", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
	hr = pDevice->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(),NULL, &pVS);
	hr = pDevice->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(),NULL, &pPS);

	pDeviceContext->VSSetShader(pVS, 0, 0);
	pDeviceContext->PSSetShader(pPS, 0, 0);

	//triangle data
	Vertex VertexData[] = 
	{
		Vertex( -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f ),
		Vertex( -0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f ),
		Vertex(  0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f ),
		Vertex(  0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f ),
	}; 

	DWORD ElementData[] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	D3D11_BUFFER_DESC iboDesc;
	ZeroMemory( &iboDesc, sizeof(iboDesc) );
	iboDesc.Usage          = D3D11_USAGE_DEFAULT;
	iboDesc.ByteWidth      = sizeof(DWORD) * 2 * 3;
	iboDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	iboDesc.CPUAccessFlags = 0;
	iboDesc.MiscFlags      = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = ElementData;
	pDevice->CreateBuffer(&iboDesc, &iinitData, &pIB);
	pDeviceContext->IASetIndexBuffer( pIB, DXGI_FORMAT_R32_UINT, 0);
	
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC vboDesc;
	ZeroMemory(&vboDesc, sizeof(vboDesc));
	vboDesc.Usage          = D3D11_USAGE_DEFAULT;
	vboDesc.ByteWidth      = sizeof(Vertex) * 4;
	vboDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	vboDesc.CPUAccessFlags = 0;
	vboDesc.MiscFlags      = 0;

	//set data to vbo
	D3D11_SUBRESOURCE_DATA vbo;
	ZeroMemory(&vbo, sizeof(vbo));
	vbo.pSysMem = VertexData;
	hr          = pDevice->CreateBuffer(&vboDesc, &vbo, &pVB);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);

	//set input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },  
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numElements = ARRAYSIZE(layout);
	pDevice->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &pInputLayout);
	pDeviceContext->IASetInputLayout(pInputLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width              = Width;
	depthStencilDesc.Height             = Height;
	depthStencilDesc.MipLevels          = 1;
	depthStencilDesc.ArraySize          = 1;
	depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags     = 0; 
	depthStencilDesc.MiscFlags          = 0;

	pDevice->CreateTexture2D(&depthStencilDesc, NULL, &pDepthStencilBuffer);
	pDevice->CreateDepthStencilView(pDepthStencilBuffer, NULL, &pDepthStencilView);
	pDeviceContext->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );


	D3D11_BUFFER_DESC mvpDesc;	
	ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));

	mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpDesc.ByteWidth      = sizeof(XMMATRIX);
	mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpDesc.CPUAccessFlags = 0;
	mvpDesc.MiscFlags      = 0;
	hr = pDevice->CreateBuffer(&mvpDesc, NULL, &pMVPBuffer);

	camPos = XMVectorSet( 0.0f, 0.0f, -5.0f, 0.0f );
	camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	camUp = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	View = XMMatrixLookAtLH( camPos, camTarget, camUp );
	Proj = XMMatrixPerspectiveFovLH( 0.4f*3.14f, (float)Width/Height, 1.0f, 1000.0f);

	//set viewpot
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = Width;
	vp.Height   = Height;
	pDeviceContext->RSSetViewports(1, &vp);

	return true;
}

bool D3DInitApp::init_shader()
{
	return true;
}