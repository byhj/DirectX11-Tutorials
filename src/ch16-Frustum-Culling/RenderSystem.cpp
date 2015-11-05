#include "RenderSystem.h"

namespace byhj
{

RenderSystem::RenderSystem() 
{
	m_AppName = L"DirectX11-Init D3D";
}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::v_Init()
{
	init_device();
	init_camera();

	m_Timer.Reset();
	m_Camera.SetRadius(10.0f);
	m_Cube.Init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get(), GetHwnd() );
	m_Font.init(m_pD3D11Device.Get());
}

void RenderSystem::v_Update()
{
	m_Cube.Update();
}

void RenderSystem::v_Render()
{
	BeginScene();

	UpdateScene();
	m_Matrix.view = m_Camera.GetViewMatrix();

	int cnt = 0;
	bool visable = true;
	for (int i = 0; i != 100; ++i)
	{
		XMMATRIX Model = XMMatrixTranslation(sin(i) * 10.0f, cos(i) *-10.0f, 0.0f);
		XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));

		XMMATRIX Proj = XMLoadFloat4x4(&m_Matrix.proj);
		XMMATRIX View = XMLoadFloat4x4(&m_Matrix.view);
	
		XMFLOAT4X4 pp;
		XMStoreFloat4x4(&pp, XMMatrixTranspose(Proj) );
		D3DXMATRIX pr = D3DXMATRIX(pp._11, pp._12, pp._13, pp._14,
			pp._21, pp._22, pp._23, pp._24,
			pp._31, pp._32, pp._33, pp._34,
			pp._41, pp._42, pp._43, pp._44);
		XMStoreFloat4x4(&pp, XMMatrixTranspose(View) );
		D3DXMATRIX vi = D3DXMATRIX(pp._11, pp._12, pp._13, pp._14,
			pp._21, pp._22, pp._23, pp._24,
			pp._31, pp._32, pp._33, pp._34,
			pp._41, pp._42, pp._43, pp._44);
		m_Frustum.Construct(1000.0f, pr, vi);
		visable = m_Frustum.CheckCube(sin(i) * 10.0f, cos(i) *-10.0f, 0.0f, 1.0f);

		if (visable)
		{
			m_Cube.Render(m_pD3D11DeviceContext.Get(), m_Matrix);
			++cnt;
		}
	}

	WCHAR cntInfo[255];
	swprintf(cntInfo, L"Count: %d", cnt);
	m_Font.drawText(m_pD3D11DeviceContext, cntInfo, 22.0f, 10.0f, 10.0f);

	EndScene();
}

void RenderSystem::v_Shutdown()
{

	m_Cube.Shutdown();
}

void RenderSystem::UpdateScene()
{
	m_Camera.update();
}
void  RenderSystem::v_OnMouseDown(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseDown(btnState, x, y, GetHwnd());
}

void  RenderSystem::v_OnMouseMove(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseMove(btnState, x, y);
}

void  RenderSystem::v_OnMouseUp(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseUp(btnState, x, y);
}
void  RenderSystem::v_OnMouseWheel(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseWheel(btnState, x, y, GetAspect());
}

void RenderSystem::init_device()
{

	////////////////////////Create buffer desc////////////////////////////
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


	///////////////////////////////////////////////////////////////////////////
	HRESULT hr;
	//Create the double buffer chain
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, NULL, NULL, NULL, D3D11_SDK_VERSION,
		&swapChainDesc, &m_pSwapChain, &m_pD3D11Device,
		NULL, &m_pD3D11DeviceContext);
	//DebugHR(hr);

	//Create back buffer, buffer also is a texture
	ID3D11Texture2D *pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	hr = m_pD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
	//DebugHR(hr);


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
	m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), NULL, &m_pDepthStencilView);
	m_pD3D11DeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());


	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode              = D3D11_CULL_BACK;
	rasterDesc.DepthBias             = 0;
	rasterDesc.DepthBiasClamp        = 0.0f;
	rasterDesc.DepthClipEnable       = true;
	rasterDesc.FillMode              = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable     = false;
	rasterDesc.ScissorEnable         = false;
	rasterDesc.SlopeScaledDepthBias  = 0.0f;
	// Create the rasterizer state from the description we just filled out.
	hr = m_pD3D11Device->CreateRasterizerState(&rasterDesc, &m_pRasterState);
	//DebugHR(hr);

	m_pD3D11DeviceContext->RSSetState(m_pRasterState.Get());
}

void RenderSystem::BeginScene()
{
	//Render 
	float bgColor[4] ={ 0.2f, 0.3f, 0.4f, 1.0f };

	m_pD3D11DeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), bgColor);
	m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

void RenderSystem::EndScene()
{
	m_pSwapChain->Present(0, 0);
}

void RenderSystem::init_camera()
{
	//Viewport Infomation
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.Width    = static_cast<FLOAT>(m_ScreenWidth);
	vp.Height   = static_cast<FLOAT>(m_ScreenHeight);
	m_pD3D11DeviceContext->RSSetViewports(1, &vp);

	//MVP Matrix
	XMVECTOR camPos    = XMVectorSet(0.0f, 0.0f, -100.0f, 0.0f);
	XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camUp     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX View      = XMMatrixLookAtLH(camPos, camTarget, camUp);
	XMMATRIX Proj      = XMMatrixPerspectiveFovLH(0.4f*3.14f, GetAspect(), 1.0f, 1000.0f);
	XMMATRIX Model     = XMMatrixIdentity();

	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model) );
	XMStoreFloat4x4(&m_Matrix.view,  XMMatrixTranspose(View) );
	XMStoreFloat4x4(&m_Matrix.proj,  XMMatrixTranspose(Proj) );

}



void RenderSystem::DrawFps()
{
	static bool flag = true;
	if (flag)
	{
		m_Timer.Start();
		flag = false;
	}

	m_Timer.Count();
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;
	if (m_Timer.GetTotalTime() - timeElapsed >= 1.0f)
	{
		fps = frameCnt;
		frameCnt = 0;
		timeElapsed += 1.0f;
	}

	m_Font.drawFps(m_pD3D11DeviceContext.Get(), (UINT)fps);
}


}