#include "RenderTextureClass.h"

RenderTextureClass::RenderTextureClass()
	:pRenderTargetTexture(0),
	 pRenderTargetView(0),
	 pShaderResourceView(0)
{

}

RenderTextureClass::RenderTextureClass(const RenderTextureClass &rhs)
{

}

RenderTextureClass::~RenderTextureClass()
{

}

bool RenderTextureClass::Init(ID3D11Device *pD3D11Device, int textureWidth, int textureHeight)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels  = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	//Create the render target texture
	result = pD3D11Device->CreateTexture2D(&textureDesc, NULL, &pRenderTargetTexture);
	if (FAILED(result))
	{
		return false;
	}

	//Setup the description of the render target view
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	result = pD3D11Device->CreateRenderTargetView(pRenderTargetTexture, &renderTargetViewDesc, &pRenderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	result = pD3D11Device->CreateShaderResourceView(pRenderTargetTexture, &shaderResourceViewDesc, &pShaderResourceView);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void RenderTextureClass::Shutdown()
{
	if (pShaderResourceView)
	{
		pShaderResourceView->Release();
		pShaderResourceView = 0;
	}
	if (pRenderTargetView)
	{
		pRenderTargetView->Release();
		pRenderTargetView = 0;
	}
	if (pRenderTargetTexture)
	{
		pRenderTargetTexture->Release();
		pRenderTargetTexture = 0;
	}

}

void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext *pD3D11DeviceContext, ID3D11DepthStencilView *pDepthStencilView)
{
	pD3D11DeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	return;
}

void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext* pD3D11DeviceContext, ID3D11DepthStencilView* pDepthStencilView, 
										   float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	pD3D11DeviceContext->ClearRenderTargetView(pRenderTargetView, color);

	// Clear the depth buffer.
	pD3D11DeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

ID3D11ShaderResourceView* RenderTextureClass::GetShaderResourceView()
{
	return pShaderResourceView;
}