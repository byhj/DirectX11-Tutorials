#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "d3d/App.h"
#include "d3d/Utility.h"
#include "Cube.h"
#include "d3d/Timer.h"
#include "d3d/Camera.h"
#include "LightShader.h"
#include "AlphaShader.h"

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
	void UpdateScene();
	void v_OnMouseDown(WPARAM btnState, int x, int y);
	void v_OnMouseMove(WPARAM btnState, int x, int y);
	void v_OnMouseUp(WPARAM btnState, int x, int y);
	void v_OnMouseWheel(WPARAM btnState, int x, int y);

private:
	void init_device();
	void init_camera();
	void BeginScene();
	void EndScene();


	byhj::Cube m_Cube;
	byhj::LightShader m_LightShader;
	byhj::AlphaShader m_AlphaShader;
	d3d::Timer m_Timer;
	d3d::Camera m_Camera;
	ComPtr<IDXGISwapChain> m_pSwapChain ;    
	ComPtr<ID3D11Device>   m_pD3D11Device;
	ComPtr<ID3D11DeviceContext> m_pD3D11DeviceContext;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
	ID3D11RasterizerState   *m_pRasterState        = nullptr;
	d3d::MatrixBuffer m_Matrix;
};
}
#endif