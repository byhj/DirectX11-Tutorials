#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "d3d/d3dApp.h"
#include "d3d/d3dUtility.h"
#include "Triangle.h"

namespace byhj
{

	class RenderSystem : public D3DApp
{
public:
	RenderSystem();
	~RenderSystem();

	void v_Init();
	void v_Render();
	void v_Shutdown();

private:
	void init_device();
	void init_camera();
	void BeginScene();
	void EndScene();


	byhj::Triangle m_Triangle;

	IDXGISwapChain          *m_pSwapChain          = nullptr;     
	ID3D11Device            *m_pD3D11Device        = nullptr;
	ID3D11DeviceContext     *m_pD3D11DeviceContext = nullptr;
	ID3D11RenderTargetView  *m_pRenderTargetView   = nullptr;
	ID3D11DepthStencilView  *m_pDepthStencilView   = nullptr;
	ID3D11Texture2D         *m_pDepthStencilBuffer = nullptr;
	MatrixBuffer m_Matrix;
};
}
#endif