#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")

#ifdef _WIN32
#define _XM_NO_INTRINSICS_
#endif 

#include "d3d/d3dApp.h"
#include "bitmap.h"
#include "cube.h"

class D3DRenderSystem: public D3DApp
{
public:
	D3DRenderSystem()
	{
		m_AppName = L"DirectX11: ch03-Init-D3D";
		m_pSwapChain          = NULL;
		m_pD3D11Device        = NULL;
		m_pD3D11DeviceContext = NULL;
		m_pRenderTargetView   = NULL;
		m_pDepthStencilView   = NULL;
		m_pDepthStencilBuffer = NULL;
		m_pRasterState        = NULL;
	}

	bool v_InitD3D();
	void v_Render();
	void v_Shutdown()
	{		
		ReleaseCOM(m_pSwapChain         )
		ReleaseCOM(m_pD3D11Device       )
		ReleaseCOM(m_pD3D11DeviceContext)
		ReleaseCOM(m_pRenderTargetView  )
		ReleaseCOM(m_pDepthStencilView  )
		ReleaseCOM(m_pDepthStencilBuffer)
		ReleaseCOM(m_pRasterState       )
	}

	ID3D11Device * GetDevice();
	ID3D11DeviceContext *GetDeviceContext();

private:
	bool init_device();
	void init_bitmap();
	void init_cube();
	void BeginScene();
	void EndScene();
	void TurnZBufferOn();
	void TurnZBufferOff();

	XMMATRIX Model;
	XMMATRIX View;
	XMMATRIX Proj;
	XMVECTOR camPos;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	IDXGISwapChain           *m_pSwapChain;
	ID3D11Device             *m_pD3D11Device;
	ID3D11DeviceContext      *m_pD3D11DeviceContext;
	ID3D11RenderTargetView   *m_pRenderTargetView;
	ID3D11DepthStencilView   *m_pDepthStencilView;
	ID3D11Texture2D          *m_pDepthStencilBuffer;
	ID3D11DepthStencilState  *m_pDepthStencilState;
	ID3D11DepthStencilState  *m_pDepthDisabledStencilState;
	ID3D11RasterizerState    *m_pRasterState;

	Bitmap bitmap;
	Cube cube;
};

CALL_MAIN(D3DRenderSystem);

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
	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format =  DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hr = m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	
	
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

	return true;
}

bool D3DRenderSystem::v_InitD3D()
{
	init_device();

	//Viewport Infomation
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = m_ScreenWidth;
	vp.Height   = m_ScreenHeight;
	m_pD3D11DeviceContext->RSSetViewports(1, &vp);


	init_bitmap();
	init_cube();
	return true;
}

void D3DRenderSystem::init_bitmap()
{
	bitmap.init_window(m_ScreenWidth, m_ScreenHeight);
	bitmap.init_buffer(m_pD3D11Device, 100, 100);
	bitmap.init_shader(m_pD3D11Device, GetHwnd());
	bitmap.init_texture(m_pD3D11Device, L"../../media/textures/stone.dds");
}

void D3DRenderSystem::init_cube()
{

	cube.init_buffer(m_pD3D11Device, m_pD3D11DeviceContext);
	cube.init_camera(GetAspect());
	cube.init_shader(m_pD3D11Device, GetHwnd());
	cube.init_texture(m_pD3D11Device, L"../../media/textures/seafloor.dds");
}

void D3DRenderSystem::v_Render()
{

   BeginScene();

   TurnZBufferOff();
   bitmap.Render(m_pD3D11DeviceContext);
   TurnZBufferOn();

   cube.Render(m_pD3D11DeviceContext);

   EndScene();
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
	float bgColor[4] = {0.0f, 0.0f, 0.5f, 1.0f};

	m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView, bgColor);
	m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void D3DRenderSystem::EndScene()
{

	m_pSwapChain->Present(0, 0);

	return;
}
