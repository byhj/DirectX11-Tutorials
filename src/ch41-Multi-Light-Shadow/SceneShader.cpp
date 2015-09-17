#include "SceneShader.h"

namespace byhj
{


	void SceneShader::Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
	{

		D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc[3];
		pInputLayoutDesc[0].SemanticName = "POSITION";
		pInputLayoutDesc[0].SemanticIndex = 0;
		pInputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		pInputLayoutDesc[0].InputSlot = 0;
		pInputLayoutDesc[0].AlignedByteOffset = 0;
		pInputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc[0].InstanceDataStepRate = 0;

		pInputLayoutDesc[1].SemanticName         = "TEXCOORD";
		pInputLayoutDesc[1].SemanticIndex        = 0;
		pInputLayoutDesc[1].Format               = DXGI_FORMAT_R32G32_FLOAT;
		pInputLayoutDesc[1].InputSlot            = 0;
		pInputLayoutDesc[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
		pInputLayoutDesc[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc[1].InstanceDataStepRate = 0;

		pInputLayoutDesc[2].SemanticName         = "NORMAL";
		pInputLayoutDesc[2].SemanticIndex        = 0;
		pInputLayoutDesc[2].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		pInputLayoutDesc[2].InputSlot            = 0;
		pInputLayoutDesc[2].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
		pInputLayoutDesc[2].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		pInputLayoutDesc[2].InstanceDataStepRate = 0;

		unsigned numElements = ARRAYSIZE(pInputLayoutDesc);

		sceneShader.init(pD3D11Device, hWnd);
		sceneShader.attachVS(L"scene.vsh", pInputLayoutDesc, numElements);
		sceneShader.attachPS(L"scene.psh");
		sceneShader.end();

		D3D11_SAMPLER_DESC samplerDesc;
		// Create a wrap texture sampler state description.
		samplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias     = 0.0f;
		samplerDesc.MaxAnisotropy  = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD         = 0;
		samplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		pD3D11Device->CreateSamplerState(&samplerDesc, &m_sampleStateWrap);
		
		// Create a clamp texture sampler state description.
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		// Create the texture sampler state.
		pD3D11Device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);

		HRESULT hr;
		D3D11_BUFFER_DESC mvpBufferDesc;
		ZeroMemory(&mvpBufferDesc, sizeof(D3D11_BUFFER_DESC));
		mvpBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
		mvpBufferDesc.ByteWidth      = sizeof(MatrixLight);
		mvpBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		mvpBufferDesc.CPUAccessFlags = 0;
		mvpBufferDesc.MiscFlags      = 0;
		hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);



		 D3D11_BUFFER_DESC lightBufferDesc;
		 // Setup the description of the light dynamic constant buffer that is in the pixel shader.
		 lightBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
		 lightBufferDesc.ByteWidth           = sizeof(LightBufferType);
		 lightBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
		 lightBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
		 lightBufferDesc.MiscFlags           = 0;
		 lightBufferDesc.StructureByteStride = 0;

		 // Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
		 pD3D11Device->CreateBuffer(&lightBufferDesc, NULL, &m_pLightBuffer);
		
		 D3D11_MAPPED_SUBRESOURCE mappedResource;

		 hr = pD3D11DeviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		 
		 LightBufferType *dataPtr1 = (LightBufferType*)mappedResource.pData;
		 dataPtr1->ambientColor = XMFLOAT4(0.15f, 0.15f, 0.15f, 0.15f);
		 dataPtr1->diffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		 dataPtr1->diffuseColor2 = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		 pD3D11DeviceContext->Unmap(m_pLightBuffer, 0);

		 int lightSlot = 0;
		 pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, &m_pLightBuffer);


		 D3D11_BUFFER_DESC lightBufferDesc2;
		 // Setup the description of the light dynamic constant buffer that is in the vertex shader.
		 lightBufferDesc2.Usage               = D3D11_USAGE_DEFAULT;
		 lightBufferDesc2.ByteWidth           = sizeof(LightBufferType2);
		 lightBufferDesc2.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
		 lightBufferDesc2.CPUAccessFlags      = 0;
		 lightBufferDesc2.MiscFlags           = 0;
		 lightBufferDesc2.StructureByteStride = 0;

		 // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		 pD3D11Device->CreateBuffer(&lightBufferDesc2, NULL, &m_pLightBuffer2);

	}

	void SceneShader::Use(ID3D11DeviceContext *pD3D11DeviceContext, const byhj::d3d::MatrixBuffer &matrix, const XMFLOAT4X4 &LightView, const XMFLOAT4X4 &LightProj,
		                  const XMFLOAT4X4 &LightView2, const XMFLOAT4X4 &LightProj2)
	{
		sceneShader.use(pD3D11DeviceContext);

		cbMatrix.model = matrix.model;
		cbMatrix.view = matrix.view;
		cbMatrix.proj = matrix.proj;
		cbMatrix.lightView = LightView;
		cbMatrix.lightPorj = LightProj;
		cbMatrix.lightView2 = LightView2;
		cbMatrix.lightProj2 = LightProj2;

		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbMatrix, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &m_pMVPBuffer);

		lightPosBuffer.lightPos = XMFLOAT3(5.0f, 8.0f, -5.0f);
		lightPosBuffer.lightPos2 = XMFLOAT3(-5.0f, 8.0f, -5.0f);
		
		pD3D11DeviceContext->UpdateSubresource(m_pLightBuffer2, 0, NULL, &lightPosBuffer, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(1, 1, &m_pLightBuffer2);

		pD3D11DeviceContext->PSSetSamplers(0, 1, &m_sampleStateClamp);
		pD3D11DeviceContext->PSSetSamplers(1, 1, &m_sampleStateWrap);
	
	}

	void SceneShader::Shutdown()
	{
		sceneShader.end();
	}

}