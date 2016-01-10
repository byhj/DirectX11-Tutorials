#include "HorizontalShader.h"

namespace byhj
{


	void HorizontalShader::Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
	{

		D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc;
		std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;

		pInputLayoutDesc.SemanticName = "POSITION";
		pInputLayoutDesc.SemanticIndex = 0;
		pInputLayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		pInputLayoutDesc.InputSlot = 0;
		pInputLayoutDesc.AlignedByteOffset = 0;
		pInputLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);

		pInputLayoutDesc.SemanticName = "TEXCOORD";
		pInputLayoutDesc.SemanticIndex = 0;
		pInputLayoutDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		pInputLayoutDesc.InputSlot = 0;
		pInputLayoutDesc.AlignedByteOffset = 12;
		pInputLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);

		

		horizontalShader.init(pD3D11Device, vInputLayoutDesc);
		horizontalShader.attachVS(L"horizontalblur.vsh", "VS", "vs_5_0");
		horizontalShader.attachPS(L"horizontalblur.psh", "PS", "ps_5_0");
		horizontalShader.end();


		HRESULT hr;
		D3D11_BUFFER_DESC mvpBufferDesc;
		ZeroMemory(&mvpBufferDesc, sizeof( D3D11_BUFFER_DESC ));
		mvpBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		mvpBufferDesc.ByteWidth = sizeof( d3d::MatrixBuffer );
		mvpBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		mvpBufferDesc.CPUAccessFlags = 0;
		mvpBufferDesc.MiscFlags = 0;
		hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);

		D3D11_BUFFER_DESC screenBufferDesc;
		ZeroMemory(&screenBufferDesc,  sizeof(D3D11_BUFFER_DESC));
		screenBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		screenBufferDesc.ByteWidth = sizeof(ScreenSize);
		screenBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hr = pD3D11Device->CreateBuffer(&screenBufferDesc, NULL, &m_pScreenBuffer);

		// Create a texture sampler state description.
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		hr = pD3D11Device->CreateSamplerState(&samplerDesc, &m_pTexSamplerState);

	}

	void HorizontalShader::Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::d3d::MatrixBuffer &matrix, 
		                       ID3D11ShaderResourceView *pTexture, const XMFLOAT4 &screenWidth)
	{
		horizontalShader.use(pD3D11DeviceContext);

		cbMatrix = matrix;
		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf());

		pD3D11DeviceContext->UpdateSubresource(m_pScreenBuffer.Get(), 0, NULL, &screenWidth, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(1, 1, m_pScreenBuffer.GetAddressOf());

		pD3D11DeviceContext->PSSetShaderResources(0, 1, &pTexture);
		pD3D11DeviceContext->PSSetSamplers(0, 1, m_pTexSamplerState.GetAddressOf());
	}

	void HorizontalShader::Shutdown()
	{
		horizontalShader.end();
	}

}