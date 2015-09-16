#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "d3d/App.h"
#include "d3d/Utility.h"
#include "Cube.h"

namespace byhj
{

	class RenderSystem : public d3d::App
{
public:
	RenderSystem();
	~RenderSystem();

	void v_Init();
	void v_Update();
	void v_Render();
	void v_Shutdown();

private:
	void init_device();
	void init_camera();
	void BeginScene();
	void EndScene();
	void TurnOnAlphaBlending()	  ;
	void TurnOffAlphaBlending()	  ;


	byhj::Cube m_Cube;

	IDXGISwapChain          *m_pSwapChain          = nullptr;     
	ID3D11Device            *m_pD3D11Device        = nullptr;
	ID3D11DeviceContext     *m_pD3D11DeviceContext = nullptr;
	ID3D11RenderTargetView  *m_pRenderTargetView   = nullptr;
	ID3D11DepthStencilView  *m_pDepthStencilView   = nullptr;
	ID3D11Texture2D         *m_pDepthStencilBuffer = nullptr;
	ID3D11RasterizerState   *m_pRasterState        = nullptr;
	ID3D11BlendState         *m_pBlendEnable;
	ID3D11BlendState         *m_pBlendDisable;

	d3d::MatrixBuffer m_Matrix;
};
}
#endif