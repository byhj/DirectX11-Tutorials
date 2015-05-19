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
	// Calculate the normal, tangent, and binormal vectors for the model.
	CalculateModelVectors();

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
	VertexType *VertexData;
	unsigned long *IndexData;
	HRESULT result;
	VertexCount = 36;
	IndexCount = 36;

	VertexData = new VertexType[VertexCount];
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
		VertexData[i].position = D3DXVECTOR3(pModelVertex[i].x, pModelVertex[i].y, pModelVertex[i].z);
		VertexData[i].texture = D3DXVECTOR2(pModelVertex[i].tu, pModelVertex[i].tv);
		VertexData[i].normal = D3DXVECTOR3(pModelVertex[i].nx, pModelVertex[i].ny, pModelVertex[i].nz);
		VertexData[i].tangent = D3DXVECTOR3(pModelVertex[i].tx, pModelVertex[i].ty, pModelVertex[i].tz);
		VertexData[i].binormal = D3DXVECTOR3(pModelVertex[i].bx, pModelVertex[i].by, pModelVertex[i].bz);

		IndexData[i] = i;
	}

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * VertexCount;
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

	pModelVertex  = new ModelType[VertexCount];
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
		fin >> pModelVertex[i].tu  >> pModelVertex[i].tv;
		fin >> pModelVertex[i].nx >> pModelVertex[i].ny >> pModelVertex[i].nz;
	}
	fin.close();

	return true;

}

void ModelClass::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;


	// Calculate the number of faces in the model.
	faceCount = VertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for(i=0; i<faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = pModelVertex[index].x;
		vertex1.y = pModelVertex[index].y;
		vertex1.z = pModelVertex[index].z;
		vertex1.tu = pModelVertex[index].tu;
		vertex1.tv = pModelVertex[index].tv;
		vertex1.nx = pModelVertex[index].nx;
		vertex1.ny = pModelVertex[index].ny;
		vertex1.nz = pModelVertex[index].nz;
		index++;

		vertex2.x = pModelVertex[index].x;
		vertex2.y = pModelVertex[index].y;
		vertex2.z = pModelVertex[index].z;
		vertex2.tu = pModelVertex[index].tu;
		vertex2.tv = pModelVertex[index].tv;
		vertex2.nx = pModelVertex[index].nx;
		vertex2.ny = pModelVertex[index].ny;
		vertex2.nz = pModelVertex[index].nz;
		index++;

		vertex3.x = pModelVertex[index].x;
		vertex3.y = pModelVertex[index].y;
		vertex3.z = pModelVertex[index].z;
		vertex3.tu = pModelVertex[index].tu;
		vertex3.tv = pModelVertex[index].tv;
		vertex3.nx = pModelVertex[index].nx;
		vertex3.ny = pModelVertex[index].ny;
		vertex3.nz = pModelVertex[index].nz;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal.
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		pModelVertex[index-1].nx = normal.x;
		pModelVertex[index-1].ny = normal.y;
		pModelVertex[index-1].nz = normal.z;
		pModelVertex[index-1].tx = tangent.x;
		pModelVertex[index-1].ty = tangent.y;
		pModelVertex[index-1].tz = tangent.z;
		pModelVertex[index-1].bx = binormal.x;
		pModelVertex[index-1].by = binormal.y;
		pModelVertex[index-1].bz = binormal.z;

		pModelVertex[index-2].nx = normal.x;
		pModelVertex[index-2].ny = normal.y;
		pModelVertex[index-2].nz = normal.z;
		pModelVertex[index-2].tx = tangent.x;
		pModelVertex[index-2].ty = tangent.y;
		pModelVertex[index-2].tz = tangent.z;
		pModelVertex[index-2].bx = binormal.x;
		pModelVertex[index-2].by = binormal.y;
		pModelVertex[index-2].bz = binormal.z;

		pModelVertex[index-3].nx = normal.x;
		pModelVertex[index-3].ny = normal.y;
		pModelVertex[index-3].nz = normal.z;
		pModelVertex[index-3].tx = tangent.x;
		pModelVertex[index-3].ty = tangent.y;
		pModelVertex[index-3].tz = tangent.z;
		pModelVertex[index-3].bx = binormal.x;
		pModelVertex[index-3].by = binormal.y;
		pModelVertex[index-3].bz = binormal.z;
	}

	return;
}
void ModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
										  VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}
void ModelClass::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
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
	stride = sizeof(VertexType);
	offset = 0;

	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return ;
}