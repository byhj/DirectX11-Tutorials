#ifndef D3DCLASS_H
#define D3DCLASS_H

#pragma  comment(lib, "dxgi.lib")
#pragma  comment(lib, "d3d11.lib")
#pragma  comment(lib, "d3dx11.lib")
#pragma  comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass &);
	~D3DClass();

	bool Init(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device * GetDevice();
	ID3D11DeviceContext *GetDeviceContext();

	void GetProj(D3DXMATRIX &);
	void GetWorld(D3DXMATRIX &);
	void GetView(D3DXMATRIX &);

	void GetVideoCardInfo(char *, int &);

	void TurnZBufferOn();
	void TurnZBufferOff();

	ID3D11DepthStencilView* GetDepthStencilView();
	void SetBackBufferRenderTarget();

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];

	IDXGISwapChain           * pSwapChain;
	ID3D11Device             * pD3D11Device;
	ID3D11DeviceContext      * pD3D11DeviceContext;
	ID3D11RenderTargetView   * pRenderTargetView;
	ID3D11Texture2D          * pDepthStencilBuffer;
	ID3D11DepthStencilState  * pDepthStencilState;
	ID3D11DepthStencilView   * pDepthStencilView;
	ID3D11RasterizerState    * pRasterState;

	ID3D11DepthStencilState  * pDepthDisabledStencilState;
	D3DXMATRIX Proj;
	D3DXMATRIX World;
	D3DXMATRIX View;

};


#endif //D3DClass