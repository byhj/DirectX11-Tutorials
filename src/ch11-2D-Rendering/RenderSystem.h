#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "d3d/App.h"
#include "d3d/Utility.h"
#include "d3d/Font.h"
#include "Cube.h"
#include "bitmap.h"

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
    	void init_object();

    	void BeginScene();
    	void EndScene();
    
    
    	byhj::Cube m_Cube;
    	byhj::Bitmap m_Bitmap;
    
    	ComPtr<IDXGISwapChain         > m_pSwapChain;
    	ComPtr<ID3D11Device           > m_pD3D11Device;
    	ComPtr<ID3D11DeviceContext    > m_pD3D11DeviceContext;
    	ComPtr<ID3D11RenderTargetView > m_pRenderTargetView;
    	ComPtr<ID3D11DepthStencilView > m_pDepthStencilView;
    	ComPtr<ID3D11Texture2D        > m_pDepthStencilBuffer;
    	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
    	ComPtr<ID3D11DepthStencilState> m_pDepthDisabledStencilState;
    	ComPtr<ID3D11RasterizerState  > m_pRasterState;
    
    	d3d::MatrixBuffer m_Matrix;
    };
}
#endif