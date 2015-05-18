#include "ModelClass.h"

ModelClass::ModelClass()
	:VertexCount(0), 
	IndexCount(0),
	pTexture(0),
	pModelVertex(0)
{
}

ModelClass::ModelClass(const ModelClass &)
{

}

ModelClass::~ModelClass()
{
}

bool ModelClass::Init(ID3D11Device *pD3D11Device, char *modelFile, WCHAR *textureFile1, WCHAR *textureFile2, WCHAR *textureFile3)
{
	bool result;

	result = LoadModel(modelFile);
	if (!result)
	{
		return false;
	}

	result = InitBuffer(pD3D11Device);
	if (!result)
	{
		return false;
	}

	//Load Texture file
	result = LoadTextures(pD3D11Device, textureFile1, textureFile2, textureFile3);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{

	ShutdownTexture();

	ShutdownBuffer();

	ShoutdownModel();

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

ID3D11ShaderResourceView **ModelClass::GetTextureArray()
{
	return pTexture->GetTextureArray();
}

bool ModelClass::InitBuffer(ID3D11Device *pD3D11Device)
{
	Vertex *VertexData;
	unsigned long *IndexData;
	HRESULT result;
	VertexCount = 36;
	IndexCount = 36;

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

	for (int i = 0; i != VertexCount; ++i)
	{
		VertexData[i].Position = D3DXVECTOR3(pModelVertex[i].x, pModelVertex[i].y, pModelVertex[i].z);
		VertexData[i].Tex      =   D3DXVECTOR2(pModelVertex[i].u, pModelVertex[i].v);
		VertexData[i].Normal   = D3DXVECTOR3(pModelVertex[i].nx, pModelVertex[i].ny, pModelVertex[i].nz);
		IndexData[i] = i;
	}

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

bool ModelClass::LoadTextures(ID3D11Device*pD3D11Device, WCHAR *textureFile1, WCHAR *textureFile2, WCHAR *textureFile3)
{
	bool result;
	pTexture = new TextureClass;
	if (!pTexture)
	{
		return false;
	}

	//Init the texture object
	result = pTexture->Init(pD3D11Device, textureFile1, textureFile2, textureFile3);
	if (!result)
	{
		return false;
	}

	return true;

}

bool ModelClass::LoadModel(char *modelFile)
{
	std::ifstream fin;
	char ch;
	int i;
	fin.open(modelFile);

	if (fin.fail())
	{
		return false;
	}
	// Read up to the value of vertex count.
	fin.get(ch);
	while(ch != ':')
	{
		fin.get(ch);
	}

	// Read in the vertex count.
	fin >> VertexCount;
	IndexCount = VertexCount;

	pModelVertex  = new ModelVertex[VertexCount];
	if (!pModelVertex)
	{
		return false;
	}

	//Read up the beginning of the data
	fin.get(ch);
	while (ch != ':')
	{
		fin.get(ch);
	}

	for (int i = 0; i != VertexCount; ++i)
	{
		fin >> pModelVertex[i].x  >> pModelVertex[i].y >> pModelVertex[i].z;
		fin >> pModelVertex[i].u  >> pModelVertex[i].v;
		fin >> pModelVertex[i].nx >> pModelVertex[i].ny >> pModelVertex[i].nz;
	}
	fin.close();

	return true;

}

void ModelClass::ShoutdownModel()
{
	if (pModelVertex)
	{
		delete [] pModelVertex;
		pModelVertex = 0;
	}
	return ;
}

void ModelClass::ShutdownTexture()
{
	if (pTexture)
	{
		pTexture->Shutdown();
		delete pTexture;
		pTexture = 0;
	}
	return ;
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