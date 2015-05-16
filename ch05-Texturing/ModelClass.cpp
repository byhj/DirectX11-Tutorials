#include "ModelClass.h"

ModelClass::ModelClass()
	:VertexCount(0), IndexCount(0)
{
}

ModelClass::ModelClass(const ModelClass &)
{

}

ModelClass::~ModelClass()
{
}

bool ModelClass::Init(ID3D11Device *pD3D11Device)
{
	bool result;
    result = InitBuffer(pD3D11Device);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	//Release the Vertex and Index Buffers
	ShutdownBuffer();

	return;
}

void ModelClass::Render(ID3D11DeviceContext *pD3D11DeviceContext)
{
	RenderBuffer(pD3D11DeviceContext);
	return ;
}

int ModelClass::GetIndexCount()
{
	return IndexCount;
}

bool ModelClass::InitBuffer(ID3D11Device *pD3D11Device)
{
	Vertex *VertexData;
	unsigned long *IndexData;
	HRESULT result;
	VertexCount = 3;
	IndexCount = 3;

	VertexData = new Vertex[VertexCount];
	if (!VertexData)
	{
		return false;
	}

	IndexData = new unsigned long[IndexCount];
	if (!IndexData)
	{
		return false;
	}

	// Load the vertex array with data.
	VertexData[0].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	VertexData[0].Color    = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	VertexData[1].Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	VertexData[1].Color    = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	VertexData[2].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	VertexData[2].Color    = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	IndexData[0] = 0;
	IndexData[1] = 1;
	IndexData[2] = 2;

   // Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(Vertex) * VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VBO;
    VBO.pSysMem = VertexData;
	VBO.SysMemPitch = 0;
	VBO.SysMemSlicePitch = 0;

	result = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &pVertexBuffer);
	if ( FAILED(result) )
	{
		return false;
	}
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA IBO;
	IBO.pSysMem = IndexData;
	IBO.SysMemPitch = 0;
	IBO.SysMemSlicePitch = 0;
	
	result = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &pIndexBuffer);
	if (FAILED (result) )
	{
		return false;
	}

	delete [] VertexData;
	VertexData = nullptr;

	delete [] IndexData;
	IndexData = nullptr;

	return true;
}

void ModelClass::ShutdownBuffer()
{
	if (pIndexBuffer)
	{
		pIndexBuffer->Release();
		pIndexBuffer = 0;
	}

	if (pVertexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer = 0;
	}
	return ;
}

void ModelClass::RenderBuffer(ID3D11DeviceContext *pD3D11DeviceContext)
{
	unsigned int stride; 
	unsigned int offset;
	stride = sizeof(Vertex);
	offset = 0;

	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return ;
}