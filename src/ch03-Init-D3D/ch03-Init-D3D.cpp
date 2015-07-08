#include "d3d/d3dApp.h"

class D3DInitApp: public D3DApp
{
public:
	D3DInitApp()
	{
		m_AppName = L"DirectX11: ch03-Init-D3D";
		m_pSwapChain          = nullptr;
		m_pD3D11Device        = nullptr;
		m_pD3D11DeviceContext = nullptr;
		m_pRenderTargetView   = nullptr;
	}

	void v_Render()
	{
		//Render 
		float bgColor[4] = {0.0f, 0.0f, 0.5f, 1.0f };

		m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
		m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView, bgColor);

		m_pSwapChain->Present(0, 0);
	}

	void v_Shutdown()
	{
		ReleaseCOM( m_pSwapChain         );
		ReleaseCOM( m_pD3D11Device       );
		ReleaseCOM( m_pD3D11DeviceContext);
		ReleaseCOM( m_pRenderTargetView  );
	}

	bool v_InitD3D();
private:

	IDXGISwapChain          *m_pSwapChain;
	ID3D11Device            *m_pD3D11Device;
	ID3D11DeviceContext     *m_pD3D11DeviceContext;
	ID3D11RenderTargetView  *m_pRenderTargetView;
};

CALL_MAIN(D3DInitApp);

bool D3DInitApp::v_InitD3D()
{
	HRESULT hr;

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

	//Create the double buffer chain
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
		                               NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, 
		                               &swapChainDesc, &m_pSwapChain, &m_pD3D11Device,
		                               NULL, &m_pD3D11DeviceContext);
	DebugHR(hr);

	//Create backbuffer, buffer also is a texture
	ID3D11Texture2D *pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	hr = m_pD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
	DebugHR(hr);

	return true;
}

