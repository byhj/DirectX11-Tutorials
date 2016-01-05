#include "PlaneShader.h"

namespace byhj
{


	void PlaneShader::Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
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
		pInputLayoutDesc.AlignedByteOffset = 0;
		pInputLayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);

		planeShader.init(pD3D11Device, vInputLayoutDesc);
		planeShader.attachVS(L"plane.vsh", "VS", "vs_5_0");
		planeShader.attachPS(L"plane.psh", "PS", "ps_5_0");
		planeShader.end();


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

	void PlaneShader::Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::d3d::MatrixBuffer &matrix)
	{
		planeShader.use(pD3D11DeviceContext);

		cbMatrix = matrix;
		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf() );
	}

	void PlaneShader::Shutdown()
	{
		planeShader.end();
	}

}