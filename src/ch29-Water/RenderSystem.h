#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "d3d/App.h"
#include "d3d/Utility.h"
#include "d3d/Font.h"
#include "d3d/Timer.h"
#include "d3d/Camera.h"
#include "d3d/d3dRTT.h"

#include "bath.h"
#include "ground.h"
#include "wall.h"
#include "water.h"

namespace byhj
{

	class RenderSystem : public d3d::App
{
public:
	RenderSystem();
	~RenderSystem();

	void v_Init();
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
	void init_object();
	void init_fbo();

	void BeginScene();
	void EndScene();
	void TurnZBufferOn();
	void TurnZBufferOff();
	void DrawFps();
	void DrawInfo(); 

	byhj::Bath   bathModel;
	byhj::Ground groundModel;
	byhj::Wall   wallModel;
	byhj::Water  waterModel;


	d3d::Font m_Font;
	d3d::Timer m_Timer;
	d3d::Camera m_Camera;
	byhj::D3DRTT    m_Rtt;
	float fps = 0.0f;
	int m_videoCardMemory;
	std::wstring m_videoCardInfo;

	IDXGISwapChain          *m_pSwapChain          = nullptr;     
	ID3D11Device            *m_pD3D11Device        = nullptr;
	ID3D11DeviceContext     *m_pD3D11DeviceContext = nullptr;
	ID3D11RenderTargetView  *m_pRenderTargetView   = nullptr;
	ID3D11DepthStencilView  *m_pDepthStencilView   = nullptr;
	ID3D11Texture2D         *m_pDepthStencilBuffer = nullptr;
	ID3D11DepthStencilState  *m_pDepthStencilState;
	ID3D11DepthStencilState  *m_pDepthDisabledStencilState;
	ID3D11RasterizerState   *m_pRasterState        = nullptr;

	ID3D11Texture2D          *m_pRttRenderTargetTexture;
	ID3D11RenderTargetView   *m_pRttRenderTargetView;
	ID3D11ShaderResourceView *m_pRttShaderResourceView;

	MatrixBuffer m_Matrix;
};
}
#endif