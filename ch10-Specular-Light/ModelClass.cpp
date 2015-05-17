#include "ModelClass.h"

ModelClass::ModelClass()
	:VertexCount(0), 
	 IndexCount(0),
	 pTexture(0)
{
}

ModelClass::ModelClass(const ModelClass &)
{

}

ModelClass::~ModelClass()
{
}

bool ModelClass::Init(ID3D11Device *pD3D11Device, WCHAR *textureFile, char *modelFile)
{
	bool result;

	result = LoadObjModel(modelFile);
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
	result = LoadTexture(pD3D11Device, textureFile);
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

ID3D11ShaderResourceView *ModelClass::GetTexture()
{
	return pTexture->GetTexture();
}

bool ModelClass::InitBuffer(ID3D11Device *pD3D11Device)
{

	HRESULT result;

   // Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(Vertex) * VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VBO;
    VBO.pSysMem = &VertexData[0];
	VBO.SysMemPitch = 0;
	VBO.SysMemSlicePitch = 0;

	result = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &pVertexBuffer);
	if ( FAILED(result) )
	{
		return false;
	}
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned int) * IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA IBO;
	IBO.pSysMem = &IndexData[0];
	IBO.SysMemPitch = 0;
	IBO.SysMemSlicePitch = 0;
	
	result = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &pIndexBuffer);
	if (FAILED (result) )
	{
		return false;
	}

    VertexData.clear();
	IndexData.clear();

	return true;
}

bool ModelClass::LoadTexture(ID3D11Device*pD3D11Device, WCHAR *textureFile)
{
	bool result;
	pTexture = new TextureClass;
	if (!pTexture)
	{
		return false;
	}

	//Init the texture object
	result = pTexture->Init(pD3D11Device, textureFile);
	if (!result)
	{
		return false;
	}

	return true;

}

bool ModelClass::LoadObjModel(char *modelFile)
{
	std::ifstream fin;
	char ch;
	fin.open(modelFile);

	if (fin.fail())
	{
       MessageBox(NULL, L"Can not open the Model File", L"Error", MB_OK | MB_ICONERROR);
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(ch);
	Vertex vt;
	D3DXVECTOR3 Pos;
	D3DXVECTOR2 Tex;
	D3DXVECTOR3 Normal;
	std::vector<D3DXVECTOR3> vPos;
	std::vector<D3DXVECTOR2> vTex;
	std::vector<D3DXVECTOR3> vNormal;
	std::vector<unsigned int> vPosIndex;
	std::vector<unsigned int> vTexIndex;
	std::vector<unsigned int> vNormalIndex;
	unsigned int a[3], b[3], c[3];
	char ct;

	while ( !fin.eof() )
	{
		if (ch == 'v')
		{
			fin.get(ch);
			if (ch == ' ')
			{
				fin >> Pos.x >> Pos.y >> Pos.z;
				Pos.z *= -1.0f;
				vPos.push_back(Pos);
			}
			if (ch == 't')
			{
				fin >> Tex.x >> Tex.y;
				Tex.y = 1.0f - Tex.y;
				vTex.push_back(Tex);
			}
			if (ch == 'n')
			{
				fin >> Normal.x >> Normal.y >> Normal.z;
				//obj is right hand, convert to lefe hand
				Normal.z *= -1.0f;
				vNormal.push_back(Normal);
			}
		}
		if (ch == 'f')
		{
			fin.get(ch);
			if (ch == ' ')
			{
				for (int i = 0; i != 3; ++i)
				{
				  fin >> a[i] >> ct >> b[i] >> ct >> c[i];
				}
				//After convert, Triangle mode is CCW, so is not show!!
				//So, we should convert the index order too!!
				for (int i = 2; i >= 0; --i)
				{
					vPosIndex.push_back(a[i]);
					vTexIndex.push_back(b[i]);
					vNormalIndex.push_back(c[i]);
				}

			}
		}
		// Read in the remainder of the line.
		while(ch != '\n')
		{
			fin.get(ch);
		}

		// Start reading the beginning of the next line.
		fin.get(ch);
	}
	fin.close();

	for (int i = 0; i != vPosIndex.size(); ++i)
	{
        vt.Position = vPos[vPosIndex[i] - 1];
		vt.Tex      = vTex[vTexIndex[i] - 1];
		vt.Normal   = vNormal[vNormalIndex[i] - 1];
		VertexData.push_back(vt);
		IndexData.push_back(i);
	}
	VertexCount = VertexData.size();
	IndexCount = IndexData.size();

	return true;

}

void ModelClass::ShoutdownModel()
{
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