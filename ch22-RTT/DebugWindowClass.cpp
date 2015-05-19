#include "DebugWindowClass.h"

DebugWindowClass::DebugWindowClass()
	:pVertexBuffer(0),
	 pIndexBuffer(0)
{

}

DebugWindowClass::DebugWindowClass(const DebugWindowClass &rhs)
{

}

DebugWindowClass::~DebugWindowClass()
{
}

bool DebugWindowClass::Init(ID3D11Device *pD3D11Device, int ScreenWidth , int ScreenHeight, int BitmapWidth, int BitmapHeight)
{
	bool result;
	screenWidth = ScreenWidth;
    screenHeight = ScreenHeight;
	bitmapWidth = BitmapWidth;
	bitmapHeight = BitmapHeight;

	lastPosX = -1;
	lastPosY = -1;

	result = InitBuffer(pD3D11Device);
    if (!result)
	{
		return false;
	}
    

	return true;
}

void DebugWindowClass::Shutdown()
{

	ShutdownBuffer();
	return;
}

bool DebugWindowClass::Render(ID3D11DeviceContext *pD3D11DeviceContext, int posX, int posY)
{
	bool result;

	result = UpdateBuffer(pD3D11DeviceContext, posX, posY);
	if (!result)
	{
		return false;
	}

	RenderBuffer(pD3D11DeviceContext);

	return true;
}

int DebugWindowClass::GetIndexCount()
{
	return IndexCount;
}


bool DebugWindowClass::InitBuffer(ID3D11Device *pD3D11Device)
{
	Vertex *VertexData;
	unsigned long *IndexData;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VBO, IBO;
	HRESULT result;
  
	VertexCount = 6;
	IndexCount = 6;

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

	memset(VertexData, 0, sizeof(Vertex) * VertexCount);

	for (int i = 0; i != IndexCount; ++i)
	{
		IndexData[i] = i;
	}
	/////////////////////////////////////////////////////////////////
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(Vertex) * VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	VBO.pSysMem = VertexData;
	VBO.SysMemPitch = 0;
	VBO.SysMemSlicePitch = 0;

	result = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &pVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//////////////////////////////////////////////////////
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	IBO.pSysMem = IndexData;
	IBO.SysMemPitch = 0;
	IBO.SysMemSlicePitch = 0;
	result = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &pIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

    delete VertexData;
	VertexData = 0;
	delete IndexData;
	IndexData = 0;
   
	return true;
}

void DebugWindowClass::ShutdownBuffer()
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

	return;
}

bool DebugWindowClass::UpdateBuffer(ID3D11DeviceContext* pD3D11DeviceContext, int posX, int posY)
{
	float left, right, top, bottom;
	Vertex *VertexData;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
    Vertex *VertexPtr;
	HRESULT result;

	//If the Position we are rendering this bitmap to has not changed then don't update
	if ( (posX == lastPosX) && (posY == lastPosY) )
	{
		return true;
	}

	lastPosX = posX;
	lastPosY = posY;

	// Calculate the screen coordinates of the left side of the bitmap.
	left = (float)((screenWidth / 2) * -1) + (float)posX;

	// Calculate the screen coordinates of the right side of the bitmap.
	right = left + (float)bitmapWidth;

	// Calculate the screen coordinates of the top of the bitmap.
	top = (float)(screenHeight / 2) - (float)posY;

	// Calculate the screen coordinates of the bottom of the bitmap.
	bottom = top - (float)bitmapHeight;

	VertexData = new Vertex[VertexCount];
	if (!VertexData)
	{
		return false;
	}

	// First triangle.
	VertexData[0].Pos = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	VertexData[0].Tex = D3DXVECTOR2(0.0f, 0.0f);

	VertexData[1].Pos = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	VertexData[1].Tex = D3DXVECTOR2(1.0f, 1.0f);

	VertexData[2].Pos = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	VertexData[2].Tex = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	VertexData[3].Pos = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	VertexData[3].Tex = D3DXVECTOR2(0.0f, 0.0f);

	VertexData[4].Pos = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	VertexData[4].Tex = D3DXVECTOR2(1.0f, 0.0f);

	VertexData[5].Pos = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	VertexData[5].Tex = D3DXVECTOR2(1.0f, 1.0f);

	result = pD3D11DeviceContext->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	VertexPtr  = (Vertex *)mappedResource.pData;
	memcpy(VertexPtr, (void*)VertexData, sizeof(Vertex) * VertexCount);
	pD3D11DeviceContext->Unmap(pVertexBuffer, 0);

	delete VertexData;
	VertexData = 0;

	return true;
}

void DebugWindowClass::RenderBuffer(ID3D11DeviceContext *pD3D11DeviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(Vertex);
	offset = 0;
	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

