#ifndef RENDERTEXTURE_CLASS
#define RENDERTEXTURE_CLASS

#include <d3d11.h>

class RenderTextureClass
{
public:
	RenderTextureClass();
	RenderTextureClass(const RenderTextureClass &);
	~RenderTextureClass();

	bool Init(ID3D11Device*, int, int);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*, float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Texture2D          *pRenderTargetTexture;
	ID3D11RenderTargetView   *pRenderTargetView;
	ID3D11ShaderResourceView *pShaderResourceView;

};

#endif