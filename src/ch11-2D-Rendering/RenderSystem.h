#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "d3d/d3dApp.h"
#include "d3d/d3dUtility.h"
#include "d3d/d3dFont.h"
#include "Cube.h"
#include "bitmap.h"

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
	void init_object();
	void BeginScene();
	void EndScene();


	byhj::Cube m_Cube;
	byhj::Bitmap m_Bitmap;

	IDXGISwapChain          *m_pSwapChain          = nullptr;     
	ID3D11Device            *m_pD3D11Device        = nullptr;
	ID3D11DeviceContext     *m_pD3D11DeviceContext = nullptr;
	ID3D11RenderTargetView  *m_pRenderTargetView   = nullptr;
	ID3D11DepthStencilView  *m_pDepthStencilView   = nullptr;
	ID3D11Texture2D         *m_pDepthStencilBuffer = nullptr;
	ID3D11DepthStencilState  *m_pDepthStencilState         = nullptr;
	ID3D11DepthStencilState  *m_pDepthDisabledStencilState = nullptr;
	ID3D11RasterizerState   *m_pRasterState        = nullptr;
	MatrixBuffer m_Matrix;
};
}
#endif