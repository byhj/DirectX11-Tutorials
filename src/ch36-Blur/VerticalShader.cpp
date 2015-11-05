#include "VerticalShader.h"

namespace byhj
{


	void VerticalShader::Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
	{

		D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc[2];
		pInputLayoutDesc[0].SemanticName = "POSITION";
		pInputLayoutDesc[0].SemanticIndex = 0;
		pInputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		pInputLayoutDesc[0].InputSlot = 0;
		pInputLayoutDesc[0].AlignedByteOffset = 0;
		pInputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc[0].InstanceDataStepRate = 0;

		pInputLayoutDesc[1].SemanticName = "TEXCOORD";
		pInputLayoutDesc[1].SemanticIndex = 0;
		pInputLayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		pInputLayoutDesc[1].InputSlot = 0;
		pInputLayoutDesc[1].AlignedByteOffset = 0;
		pInputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc[1].InstanceDataStepRate = 0;


		

		verticalShader.init(pD3D11Device, vInputLayoutDesc);
		verticalShader.attachVS(L"verticalblur.vsh", "VS", "vs_5_0");
		verticalShader.attachPS(L"verticalblur.psh");
		verticalShader.end();


		HRESULT hr;
		D3D11_BUFFER_DESC mvpBufferDesc;
		ZeroMemory(&mvpBufferDesc, sizeof( D3D11_BUFFER_DESC ));
		mvpBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		mvpBufferDesc.ByteWidth = sizeof( d3d::MatrixBuffer );
		mvpBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		mvpBufferDesc.CPUAccessFlags = 0;
		mvpBufferDesc.MiscFlags = 0;
		hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);

	}

	void VerticalShader::Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::d3d::MatrixBuffer &matrix)
	{
		verticalShader.use(pD3D11DeviceContext);

		cbMatrix = matrix;
		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf() );
	}

	void VerticalShader::Shutdown()
	{
		verticalShader.end();
	}

}