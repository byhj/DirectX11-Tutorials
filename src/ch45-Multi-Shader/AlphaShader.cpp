#include "AlphaShader.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{
	void AlphaShader::Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
	{
		init_shader(pD3D11Device, hWnd);
		init_texture(pD3D11Device);
		init_buffer(pD3D11Device, pD3D11DeviceContext);
	}

	void AlphaShader::Render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix)
	{
		cbMatrix.model = matrix.model;
		cbMatrix.view  = matrix.view;
		cbMatrix.proj  = matrix.proj;
		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf() );
		pD3D11DeviceContext->PSSetShaderResources(0, 3, m_pTextures);
		pD3D11DeviceContext->PSSetSamplers(0, 1, m_pTexSamplerState.GetAddressOf());

		CubeShader.use(pD3D11DeviceContext);
	}

	void AlphaShader::Shutdown()
	{

	}


	void AlphaShader::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
	{
		D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc;
		std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;

		pInputLayoutDesc.SemanticName         = "POSITION";
		pInputLayoutDesc.SemanticIndex        = 0;
		pInputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		pInputLayoutDesc.InputSlot            = 0;
		pInputLayoutDesc.AlignedByteOffset    = 0;
		pInputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);

		pInputLayoutDesc.SemanticName         = "TEXCOORD";
		pInputLayoutDesc.SemanticIndex        = 0;
		pInputLayoutDesc.Format               = DXGI_FORMAT_R32G32_FLOAT;
		pInputLayoutDesc.InputSlot            = 0;
		pInputLayoutDesc.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
		pInputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);

		pInputLayoutDesc.SemanticName         = "NORMAL";
		pInputLayoutDesc.SemanticIndex        = 0;
		pInputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		pInputLayoutDesc.InputSlot            = 0;
		pInputLayoutDesc.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
		pInputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(pInputLayoutDesc);
		

		CubeShader.init(pD3D11Device, vInputLayoutDesc);
		CubeShader.attachVS(L"alpha.vsh", "VS", "vs_5_0");
		CubeShader.attachPS(L"alpha.psh", "PS", "ps_5_0");
		CubeShader.end();
	}

	void AlphaShader::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		////////////////////////////////Const Buffer//////////////////////////////////////

		HRESULT hr;

		D3D11_BUFFER_DESC mvpDesc;
		ZeroMemory(&mvpDesc, sizeof( D3D11_BUFFER_DESC ));
		mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
		mvpDesc.ByteWidth      = sizeof( d3d::MatrixBuffer );
		mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		mvpDesc.CPUAccessFlags = 0;
		mvpDesc.MiscFlags      = 0;
		hr = pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);
		//DebugHR(hr);


		/////////////////////////////////////////////////////////////////////////////////
		D3D11_BUFFER_DESC lightBufferDesc;
		ZeroMemory(&lightBufferDesc, sizeof( D3D11_BUFFER_DESC ));
		lightBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
		lightBufferDesc.ByteWidth      = sizeof( LightBuffer );
		lightBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc.MiscFlags      = 0;

		hr = pD3D11Device->CreateBuffer(&lightBufferDesc, NULL, &m_pLightBuffer);
		//DebugHR(hr);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		// Lock the light constant buffer so it can be written to.
		hr = pD3D11DeviceContext->Map(m_pLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		//DebugHR(hr);

		// Get a pointer to the data in the constant buffer.
		LightBuffer *dataPtr2 = ( LightBuffer* )mappedResource.pData;

		dataPtr2->ambientColor   = XMFLOAT4(0.15f, 0.15f, 0.15f, 0.15f);
		dataPtr2->diffuseColor   = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		dataPtr2->lightDirection = XMFLOAT3(0.0f, 2.0f, 1.0f);

		pD3D11DeviceContext->Unmap(m_pLightBuffer.Get(), 0);

		int lightSlot = 0;
		pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, m_pLightBuffer.GetAddressOf());


	}
	void AlphaShader::init_texture(ID3D11Device *pD3D11Device)
	{

		HRESULT hr;
		hr = CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/stone01.dds", NULL, &m_pTextures[0]);
		//DebugHR(hr);
		hr = CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/dirt01.dds", NULL, &m_pTextures[1]);
		//DebugHR(hr);
		hr = CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/alpha01.dds", NULL, &m_pTextures[2]);
		//DebugHR(hr);

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
		//DebugHR(hr);
	}
}

