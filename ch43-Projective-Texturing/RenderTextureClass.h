#ifndef RENDERTEXTURE_CLASS
#define RENDERTEXTURE_CLASS

#include <d3d11.h>
#include <d3dx10math.h>

class RenderTextureClass
{
public:
	RenderTextureClass();
	RenderTextureClass(const RenderTextureClass &);
	~RenderTextureClass();

	bool Init(ID3D11Device*, int, int, float, float);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*);
	void ClearRenderTarget(ID3D11DeviceContext*, float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();
	void GetProjectionMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

private:
	ID3D11Texture2D          *pRenderTargetTexture;
	ID3D11RenderTargetView   *pRenderTargetView;
	ID3D11ShaderResourceView *pShaderResourceView;

	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_orthoMatrix;
};

#endif