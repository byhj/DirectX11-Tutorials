#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

#ifdef _WIN32
#define _XM_NO_INTRINSICS_
#endif 

#include "common/d3dApp.h"
#include "common/d3dFont.h"
#include "common/d3dTimer.h"
#include "common/d3dFrustum.h"

#include "cube.h"

const float Pi = 3.1415926535f;

float Clamp(const float& x, const float& low, const float& high)
{
	return x < low ? low : (x > high ? high : x); 
}

class D3DRenderSystem: public D3DApp
{
public:
	D3DRenderSystem():m_Theta(1.5f * Pi), m_Phi(0.25f * Pi), m_Radius(10.0f) 
	{
		m_AppName = L"DirectX11: ch12-Font";
		m_pSwapChain          = NULL;
		m_pD3D11Device        = NULL;
		m_pD3D11DeviceContext = NULL;
		m_pRenderTargetView   = NULL;
		m_pDepthStencilView   = NULL;
		m_pDepthStencilBuffer = NULL;
		m_pMVPBuffer          = NULL;
		m_pRasterState        = NULL;
		fps = 0.0f;
		m_LastMousePos.x = 0;
		m_LastMousePos.y = 0;

		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&m_World, I);
		XMStoreFloat4x4(&m_View, I);
		XMStoreFloat4x4(&m_Proj, I);
	}

	bool v_InitD3D();
	void v_Render();
	void v_OnMouseDown(WPARAM btnState, int x, int y);
	void v_OnMouseMove(WPARAM btnState, int x, int y);
	void v_OnMouseUp(WPARAM btnState, int x, int y);
	void v_OnMouseWheel(WPARAM btnState, int x, int y);

	void update();

	void v_Shutdown()
	{		
		ReleaseCOM(m_pSwapChain         )
			ReleaseCOM(m_pD3D11Device       )
			ReleaseCOM(m_pD3D11DeviceContext)
			ReleaseCOM(m_pRenderTargetView  )
			ReleaseCOM(m_pDepthStencilView  )
			ReleaseCOM(m_pDepthStencilBuffer)
			ReleaseCOM(m_pMVPBuffer         )
			ReleaseCOM(m_pRasterState       )
	}

	ID3D11Device * GetDevice();
	ID3D11DeviceContext *GetDeviceContext();

private:
	bool init_device();
	void init_camera();
	void init_object();

	void BeginScene();
	void EndScene();
	void TurnZBufferOn();
	void TurnZBufferOff();
	void DrawFps();
	void DrawMessage();

	XMMATRIX Model;
	XMMATRIX View;
	XMMATRIX Proj;
	XMVECTOR camPos;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	//D3D Device 
	IDXGISwapChain           *m_pSwapChain;
	ID3D11Device             *m_pD3D11Device;
	ID3D11DeviceContext      *m_pD3D11DeviceContext;
	ID3D11RenderTargetView   *m_pRenderTargetView;
	ID3D11DepthStencilView   *m_pDepthStencilView;
	ID3D11Texture2D          *m_pDepthStencilBuffer;
	ID3D11DepthStencilState  *m_pDepthStencilState;
	ID3D11DepthStencilState  *m_pDepthDisabledStencilState;
	ID3D11Buffer             *m_pMVPBuffer;
	ID3D11RasterizerState    *m_pRasterState;

	D3DFont font;
	Cube cube;
	D3DFrustum frustum;
	D3DTimer timer;
	int m_videoCardMemory;
	WCHAR m_videoCardInfo[255];
	float fps;

	XMFLOAT4X4 m_World;
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;

	float m_Theta;
	float m_Phi;
	float m_Radius;

	POINT m_LastMousePos;

};

CALL_MAIN(D3DRenderSystem);

void D3DRenderSystem::update()
{
	// Convert Spherical to Cartesian coordinates.
	float x = m_Radius * sinf(m_Phi) * cosf(m_Theta);
	float z = m_Radius * sinf(m_Phi) * sinf(m_Theta);
	float y = m_Radius * cosf(m_Phi);

	// Build the view matrix.
	XMVECTOR pos    = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&m_View, V);
}

void D3DRenderSystem::v_OnMouseWheel(WPARAM btnState, int x, int y)
{
	static float zoom = 45.0f;
	zoom += x * 0.01f;
	Proj   = XMMatrixPerspectiveFovLH( D3DXToRadian(zoom), GetAspect(), 1.0f, 1000.0f);
}

void D3DRenderSystem::v_OnMouseDown(WPARAM btnState, int x, int y)
{
	m_LastMousePos.x = x;
	m_LastMousePos.y = y;

	SetCapture( GetHwnd() );
}

void D3DRenderSystem::v_OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void D3DRenderSystem::v_OnMouseMove(WPARAM btnState, int x, int y)
{
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - m_LastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - m_LastMousePos.y));

		// Update angles based on input to orbit camera around box.
		m_Theta += dx;
		m_Phi   += dy;

		// Restrict the angle mPhi.
		m_Phi = Clamp(m_Phi, 0.1f, Pi-0.1f);
	}

	else if( (btnState & MK_RBUTTON) != 0 )
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f*static_cast<float>(x - m_LastMousePos.x);
		float dy = 0.005f*static_cast<float>(y - m_LastMousePos.y);

		// Update the camera radius based on input.
		m_Radius += dx - dy;

		// Restrict the radius.
		m_Radius = Clamp(m_Radius, 3.0f, 15.0f);
	}

	m_LastMousePos.x = x;
	m_LastMousePos.y = y;
}

bool D3DRenderSystem::v_InitD3D()
{
	init_device();
	init_camera();
	init_object();
	//camera.InitDirectInput(GetHwnd(), GetAppInst());

	return true;
}

void D3DRenderSystem::v_Render()
{

	BeginScene();

	static float rot = 0.0f;
	rot +=  timer.GetDeltaTime();
	update();
	View = XMLoadFloat4x4(&m_View);

	//Model = XMMatrixRotationY(rot);
	int cnt = 0;
	bool visable = true;
	for (int i = 0; i != 100; ++i)
	{
		Model = XMMatrixTranslation(sin(i) * 10.0f, cos(i) *-10.0f, 0.0f);
		XMFLOAT4X4 pp;
		XMStoreFloat4x4(&pp, Proj);
		D3DXMATRIX pr = D3DXMATRIX(pp._11, pp._12, pp._13, pp._14,
			                       pp._21, pp._22, pp._23, pp._24,
								   pp._31, pp._32, pp._33, pp._34,
								   pp._41, pp._42, pp._43, pp._44);
		XMStoreFloat4x4(&pp, View);
		D3DXMATRIX vi = D3DXMATRIX(pp._11, pp._12, pp._13, pp._14,
			pp._21, pp._22, pp._23, pp._24,
			pp._31, pp._32, pp._33, pp._34,
			pp._41, pp._42, pp._43, pp._44);
		frustum.Construct(1000.0f, pr, vi);
		visable = frustum.CheckCube(sin(i) * 10.0f, cos(i) *-10.0f, 0.0f, 1.0f);

		if (visable)
		{
	      cube.Render(m_pD3D11DeviceContext, Model, View, Proj);
		  ++cnt;
		}
	}

	DrawMessage();
	WCHAR cntInfo[255];
	swprintf(cntInfo, L"Count: %d", cnt);
	font.drawText(m_pD3D11DeviceContext, cntInfo, 22.0f, 10.0f, 100.0f);
	EndScene();
}

bool D3DRenderSystem::init_device()
{
	HRESULT hr;

	////////////////////////////Create buffer desc//////////////////////////
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width                   = m_ScreenWidth;
	bufferDesc.Height                  = m_ScreenHeight;
	bufferDesc.RefreshRate.Numerator   = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;

	///////////////////////////Create swapChain Desc////////////////////////
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
	// Initialize the description of the depth buffer.

	///////////////////// Set up the description of the depth buffer.////////////////////////
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width              = m_ScreenWidth;
	depthBufferDesc.Height             = m_ScreenHeight;
	depthBufferDesc.MipLevels          = 1;
	depthBufferDesc.ArraySize          = 1;
	depthBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count   = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage              = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags     = 0;
	depthBufferDesc.MiscFlags          = 0;
	// Create the texture for the depth buffer using the filled out description.
	hr = m_pD3D11Device->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);


	//////////////////////////// Initialize the description of the stencil state.///////////////////////////////////////////////
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable      = true;
	depthStencilDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc        = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable    = true;
	depthStencilDesc.StencilReadMask  = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	hr = m_pD3D11Device->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	// Set the depth stencil state.

	DebugHR(hr);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format =  DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hr = m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	DebugHR(hr);

	// ////////////Clear the second depth stencil state before setting the parameters.//////////////////////
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	hr = m_pD3D11Device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_pDepthDisabledStencilState);
	DebugHR(hr);

	unsigned int numModes, i, numerator, denominator, stringLength;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGISurface *surface;
	DXGI_ADAPTER_DESC adapterDesc;

	// Create a DirectX graphics interface factory.
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	// Use the factory to create an adapter for the primary graphics interface (video card).
	factory->EnumAdapters(0, &adapter);
	adapter->GetDesc(&adapterDesc);
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	swprintf(m_videoCardInfo,L"Video Card  : %ls", adapterDesc.Description);
	return true;
}

void D3DRenderSystem::init_camera()
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
	Proj      = XMMatrixPerspectiveFovLH( D3DXToRadian(45.0f), GetAspect(), 1.0f, 1000.0f);
	Model     = XMMatrixIdentity();
}

void D3DRenderSystem::init_object()
{

	cube.init_buffer(m_pD3D11Device, m_pD3D11DeviceContext);
	cube.init_shader(m_pD3D11Device, GetHwnd());
	cube.init_texture(m_pD3D11Device, L"../../media/textures/stone01.dds");
	font.init(m_pD3D11Device);

	timer.Reset();
}



void D3DRenderSystem::TurnZBufferOn()
{
	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	return;
}


void D3DRenderSystem::TurnZBufferOff()
{
	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthDisabledStencilState, 1);
	return;
}

void  D3DRenderSystem::BeginScene()
{
	D3DXVECTOR4 bgColor = D3DXVECTOR4(0.2f, 0.3f, 0.4f, 1.0f);

	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	m_pD3D11DeviceContext->OMSetBlendState(NULL, NULL, 0XFFFFFFFF);
	m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView, bgColor);
	m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void D3DRenderSystem::EndScene()
{

	m_pSwapChain->Present(0, 0);

	return;
}


void D3DRenderSystem::DrawFps()
{
	static bool flag = true;
	if (flag)
	{
		timer.Start();
		flag = false;
	}

	timer.Count();
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;
	if(timer.GetTotalTime() - timeElapsed >= 1.0f)
	{
		fps = frameCnt;
		frameCnt = 0;
		timeElapsed += 1.0f;
	}	

	font.drawFps(m_pD3D11DeviceContext, (UINT)fps);
}

void D3DRenderSystem::DrawMessage()
{
	WCHAR WinInfo[255];
	swprintf(WinInfo, L"Window Size: %d x %d", m_ScreenWidth, m_ScreenHeight);
	DrawFps();
	font.drawText(m_pD3D11DeviceContext, WinInfo, 22.0f, 10.0f, 40.0f);
	font.drawText(m_pD3D11DeviceContext, m_videoCardInfo, 22.0f, 10.0f, 70.0f);
}
