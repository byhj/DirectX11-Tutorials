#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "d3d/d3dApp.h"
#include "d3d/d3dUtility.h"
#include "d3d/d3dFont.h"
#include "d3d/d3dTimer.h"
#include "d3d/d3dInput.h"
#include "d3d/d3dSound.h"

#include "Cube.h"

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
	void DrawFps();

	byhj::Cube m_Cube;
	byhj::D3DFont m_Font;
	byhj::D3DTimer m_Timer;
	byhj::D3DCamera m_Camera;
	byhj::D3DSound m_Sound;

	float fps = 0.0f;

	IDXGISwapChain          *m_pSwapChain          = nullptr;     
	ID3D11Device            *m_pD3D11Device        = nullptr;
	ID3D11DeviceContext     *m_pD3D11DeviceContext = nullptr;
	ID3D11RenderTargetView  *m_pRenderTargetView   = nullptr;
	ID3D11DepthStencilView  *m_pDepthStencilView   = nullptr;
	ID3D11Texture2D         *m_pDepthStencilBuffer = nullptr;
	ID3D11RasterizerState   *m_pRasterState        = nullptr;
	MatrixBuffer m_Matrix;
};
}
#endif