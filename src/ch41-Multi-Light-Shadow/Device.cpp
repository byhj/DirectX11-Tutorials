#include "Device.h"

namespace byhj
{
	void Device::Init(float sw, float sh, HWND hWnd)
	{
		m_ScreenWidth = sw;
		m_ScreenHeight = sh;
		init_device(hWnd);
		init_camera();
		init_fbo();

		m_Timer.Reset();
		m_Font.init(m_pD3D11Device.Get());
	}

	void Device::Update()
	{

	}
	float Device::GetDeltaTime()
	{
		return m_Timer.GetDeltaTime();
	}
	void Device::Render()
	{
	}

	void Device::Shutdown()
	{

	}
	void Device::init_device(HWND hWnd)
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
		swapChainDesc.OutputWindow       = hWnd;
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

		//////////////////////////// Initialize the description of the stencil state.///////////////////////////////////////////////
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		depthBufferDesc.Width = m_ScreenWidth;
		depthBufferDesc.Height = m_ScreenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		// Create the texture for the depth buffer using the filled out description.
		hr = m_pD3D11Device->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);


		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		hr = m_pD3D11Device->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
		// Set the depth stencil state.

		//DebugHR(hr);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		// Initialize the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		hr = m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), &depthStencilViewDesc, &m_pDepthStencilView);
		//DebugHR(hr);

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
		//DebugHR(hr);

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

		///////////////////////////////////////////////////////////////////////////////

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
		m_videoCardInfo = std::wstring(L"Video Card  :") + adapterDesc.Description;

	}
	void Device::BeginScene()
	{
		//Render 
		float bgColor[4] ={ 0.0f, 0.0f, 0.0f, 1.0f };

		m_pD3D11DeviceContext->RSSetState(m_pRasterState.Get());
		m_pD3D11DeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
		m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
		m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), bgColor);
		m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	}

	void Device::EndScene()
	{
		m_pSwapChain->Present(0, 0);
	}

	void Device::BeginRTT1()
	{
		float bgColor[4] ={ 0.0f, 0.0f, 0.0f, 1.0f };

		m_pD3D11DeviceContext->OMSetRenderTargets(1, m_pRttRenderTargetViews[0].GetAddressOf(), m_pDepthStencilView.Get());
		m_pD3D11DeviceContext->ClearRenderTargetView(m_pRttRenderTargetViews[0].Get(), bgColor);
		m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void Device::BeginRTT2()
	{
		float bgColor[4] ={ 0.0f, 0.0f, 0.0f, 1.0f };

		m_pD3D11DeviceContext->OMSetRenderTargets(1, m_pRttRenderTargetViews[1].GetAddressOf(), m_pDepthStencilView.Get());
		m_pD3D11DeviceContext->ClearRenderTargetView(m_pRttRenderTargetViews[1].Get(), bgColor);
		m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	ID3D11Device * Device::GetDevice()
	{
		return m_pD3D11Device.Get();
	}

	ID3D11DeviceContext * Device::GetDeviceContext()
	{
		return m_pD3D11DeviceContext.Get();
	}
	ID3D11ShaderResourceView ** Device::GetSRV1() 
	{
		return m_pRttShaderResourceViews[0].GetAddressOf();
	}
	ID3D11ShaderResourceView ** Device::GetSRV2()
	{
		return m_pRttShaderResourceViews[1].GetAddressOf();
	}
	void Device::EnableZBuffer()
	{
		m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	}


	void Device::DisableZBuffer()
	{
		m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthDisabledStencilState.Get(), 1);
	}
	void Device::init_camera()
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
		auto aspect = m_ScreenWidth / m_ScreenHeight;
		XMVECTOR camPos    = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
		XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR camUp     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX View      = XMMatrixLookAtLH(camPos, camTarget, camUp);
		XMMATRIX Proj      = XMMatrixPerspectiveFovLH(0.4f*3.14f, aspect, 1.0f, 1000.0f);
		XMMATRIX Model     = XMMatrixIdentity();

		XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
		XMStoreFloat4x4(&m_Matrix.view, XMMatrixTranspose(View));
		XMStoreFloat4x4(&m_Matrix.proj, XMMatrixTranspose(Proj));
	}


	void Device::init_fbo()
	{

		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT result;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = m_ScreenWidth;
		textureDesc.Height = m_ScreenHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		//Create the render target texture
		result = m_pD3D11Device->CreateTexture2D(&textureDesc, NULL, &m_pRttRenderTargetTextures[0]);
		result = m_pD3D11Device->CreateTexture2D(&textureDesc, NULL, &m_pRttRenderTargetTextures[1]);


		//Setup the description of the render target view
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		result = m_pD3D11Device->CreateRenderTargetView(m_pRttRenderTargetTextures[0].Get(), &renderTargetViewDesc, m_pRttRenderTargetViews[0].GetAddressOf());
		result = m_pD3D11Device->CreateRenderTargetView(m_pRttRenderTargetTextures[1].Get(), &renderTargetViewDesc, m_pRttRenderTargetViews[1].GetAddressOf());

		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		result = m_pD3D11Device->CreateShaderResourceView(m_pRttRenderTargetTextures[0].Get(), &shaderResourceViewDesc, m_pRttShaderResourceViews[0].GetAddressOf());
		result = m_pD3D11Device->CreateShaderResourceView(m_pRttRenderTargetTextures[1].Get(), &shaderResourceViewDesc, m_pRttShaderResourceViews[1].GetAddressOf());

	}


	void Device::DrawFps()
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

	void Device::DrawInfo()
	{
		WCHAR WinInfo[255];
		swprintf(WinInfo, L"Window Size: %d x %d", m_ScreenWidth, m_ScreenHeight);
		DrawFps();
		m_Font.drawText(m_pD3D11DeviceContext.Get(), WinInfo, 22.0f, 10.0f, 40.0f);
		m_Font.drawText(m_pD3D11DeviceContext.Get(), m_videoCardInfo.c_str(), 22.0f, 10.0f, 70.0f);
	}
}