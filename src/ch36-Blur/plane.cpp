#include "Plane.h"

namespace byhj
{


void Plane::init_window(float posX, float posY, float width, float height, float aspect)
{
	m_posX  = posX;
	m_posY  = posY;
	m_width = width; 
	m_height = height;
	m_aspect = aspect;
}

void Plane::Render(ID3D11DeviceContext *pD3D11DeviceContext, ID3D11ShaderResourceView *pTexture)
{

	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex); 
	offset = 0;

	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pD3D11DeviceContext->PSSetShaderResources(0, 1, &pTexture);  
	pD3D11DeviceContext->PSSetSamplers( 0, 1, &m_pTexSamplerState );

	pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

}

bool Plane::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	Vertex *VertexData;
	unsigned long *IndexData;
	m_VertexCount = 6;
	m_IndexCount = 6;

	VertexData = new Vertex[m_VertexCount];
	if (!VertexData)
	{
		return false;
	}

	IndexData = new unsigned long[m_IndexCount];
	if (!IndexData)
	{
		return false;
	}

	memset(VertexData, 0, sizeof(Vertex) * m_VertexCount);

	for (int i = 0; i != m_IndexCount; ++i)
	{
		IndexData[i] = i;
	}

	// First triangle.
	VertexData[0].Pos = XMFLOAT3(m_posX, m_posY, 0.0f);  // Top left.
	VertexData[0].Tex = XMFLOAT2(0.0f, 0.0f);

	VertexData[1].Pos = XMFLOAT3(m_posX + m_width, m_posY, 0.0f);  
	VertexData[1].Tex = XMFLOAT2(1.0f, 0.0f);

	VertexData[2].Pos = XMFLOAT3(m_posX + m_width, m_posY - m_height, 0.0f);   //Bottom right
	VertexData[2].Tex = XMFLOAT2(1.0f, 1.0f);

	// Second triangle.
	VertexData[3].Pos = XMFLOAT3(m_posX + m_width, m_posY - m_height, 0.0f);   
	VertexData[3].Tex = XMFLOAT2(1.0f, 1.0f);

	VertexData[4].Pos = XMFLOAT3(m_posX, m_posY - m_height, 0.0f); 
	VertexData[4].Tex = XMFLOAT2(0.0f, 1.0f);

	VertexData[5].Pos = XMFLOAT3(m_posX, m_posY, 0.0f); 
	VertexData[5].Tex = XMFLOAT2(0.0f, 0.0f);


	///////////////////////////Index Buffer ////////////////////////////////

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth           = sizeof(Vertex) * m_VertexCount;
	VertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags      = 0;
	VertexBufferDesc.MiscFlags           = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA VBO;
	VBO.pSysMem          = VertexData;
	VBO.SysMemPitch      = 0;
	VBO.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	hr = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
	DebugHR(hr);

	/////////////////////////////////Index Buffer ///////////////////////////////////////

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_IndexCount;
	IndexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags      = 0;
	IndexBufferDesc.MiscFlags           = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA IBO;
	IBO.pSysMem          = IndexData;
	IBO.SysMemPitch      = 0;
	IBO.SysMemSlicePitch = 0;

	hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
	DebugHR(hr);

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
	DebugHR(hr);

	return true;
}


bool Plane::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{

	return true;
}

}
