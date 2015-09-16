#include "Cube.h"
#include "d3d/d3dDebug.h"

namespace byhj
{

Cube::Cube()
{

}

Cube::~Cube()
{

}

void Cube::Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
{
 
	init_buffer(pD3D11Device, pD3D11DeviceContext);
	init_shader(pD3D11Device, hWnd);
	init_texture(pD3D11Device);
}

void Cube::Render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix)
{


	cbMatrix.model = matrix.model;
	cbMatrix.view  = matrix.view;
	cbMatrix.proj  = matrix.proj;
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbMatrix, 0, 0);
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &m_pMVPBuffer);

	// Set vertex buffer stride and offset
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(VertexType);
	offset = 0;
	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pD3D11DeviceContext->PSSetShaderResources(0, 3, m_pTextures);
	pD3D11DeviceContext->PSSetSamplers(0, 1, &m_pTexSamplerState);

	CubeShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);


}

void Cube::Shutdown()
{
	ReleaseCOM(m_pInputLayout)
	ReleaseCOM(m_pVS)
	ReleaseCOM(m_pPS)
	ReleaseCOM(m_pMVPBuffer)
	ReleaseCOM(m_pVertexBuffer)
	ReleaseCOM(m_pIndexBuffer)
	ReleaseCOM(m_pLightBuffer)
	ReleaseCOM(m_CameraBuffer)
	ReleaseCOM(m_pTexSamplerState)
	//ReleaseCOM(m_pTexture)

}

void Cube::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;
	///////////////////////////Index Buffer ////////////////////////////////
	LoadModel("../../media/objects/cube.txt");
	CalculateModelVectors();

	VertexType *VertexData;
	unsigned long *IndexData;
	HRESULT result;
	int VertexCount = 36;
	int IndexCount = 36;

	VertexData = new VertexType[VertexCount];
	IndexData = new unsigned long[IndexCount];


	for (int i = 0; i != VertexCount; ++i)
	{
		VertexData[i].position = XMFLOAT3(pModelVertex[i].x, pModelVertex[i].y, pModelVertex[i].z);
		VertexData[i].texture = XMFLOAT2(pModelVertex[i].tu, pModelVertex[i].tv);
		VertexData[i].normal = XMFLOAT3(pModelVertex[i].nx, pModelVertex[i].ny, pModelVertex[i].nz);
		VertexData[i].tangent = XMFLOAT3(pModelVertex[i].tx, pModelVertex[i].ty, pModelVertex[i].tz);
		VertexData[i].binormal = XMFLOAT3(pModelVertex[i].bx, pModelVertex[i].by, pModelVertex[i].bz);

		IndexData[i] = i;
	}

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth           = sizeof(VertexType)* VertexCount;
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
	IndexBufferDesc.ByteWidth           = sizeof(unsigned long)* IndexCount;
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
	////////////////////////////////Const Buffer//////////////////////////////////////

	D3D11_BUFFER_DESC mvpDesc;
	ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
	mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpDesc.ByteWidth      = sizeof(d3d::MatrixBuffer);
	mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpDesc.CPUAccessFlags = 0;
	mvpDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);
	DebugHR(hr);


	/////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(D3D11_BUFFER_DESC));
	lightBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth      = sizeof(LightBuffer);
	lightBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags      = 0;

	hr = pD3D11Device->CreateBuffer(&lightBufferDesc, NULL, &m_pLightBuffer);
	DebugHR(hr);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// Lock the light constant buffer so it can be written to.
	hr = pD3D11DeviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	DebugHR(hr);

	// Get a pointer to the data in the constant buffer.
	LightBuffer *dataPtr2 = (LightBuffer*)mappedResource.pData;


	dataPtr2->ambientColor   = XMFLOAT4(0.15f, 0.15f, 0.15f, 0.15f);
	dataPtr2->diffuseColor   = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	dataPtr2->lightDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	dataPtr2->specularPower  = 32.0f;
	dataPtr2->specularColor  = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	pD3D11DeviceContext->Unmap(m_pLightBuffer, 0);

	int lightSlot = 0;
	pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, &m_pLightBuffer);


	D3D11_BUFFER_DESC cameraBufferDesc;
	cameraBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth           = sizeof(CameraBuffer);
	cameraBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags           = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = pD3D11Device->CreateBuffer(&cameraBufferDesc, NULL, &m_CameraBuffer);
	DebugHR(hr);

	// Lock the camera constant buffer so it can be written to.
	hr = pD3D11DeviceContext->Map(m_CameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	DebugHR(hr);

	// Get a pointer to the data in the constant buffer.
	CameraBuffer *dataPtr3 = (CameraBuffer*)mappedResource.pData;
	dataPtr3->camPos = XMFLOAT3(0.0f, 2.0f, -3.0f);
	dataPtr3->padding = 0.0f;
	pD3D11DeviceContext->Unmap(m_CameraBuffer, 0);

	int bufferSlot = 1;
	pD3D11DeviceContext->VSSetConstantBuffers(bufferSlot, 1, &m_CameraBuffer);


}

void Cube::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
	D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc[5];
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
	pInputLayoutDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[1].InstanceDataStepRate = 0;

	pInputLayoutDesc[2].SemanticName = "NORMAL";
	pInputLayoutDesc[2].SemanticIndex = 0;
	pInputLayoutDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[2].InputSlot = 0;
	pInputLayoutDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[2].InstanceDataStepRate = 0;

	pInputLayoutDesc[3].SemanticName = "TANGENT";
	pInputLayoutDesc[3].SemanticIndex = 0;
	pInputLayoutDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[3].InputSlot = 0;
	pInputLayoutDesc[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[3].InstanceDataStepRate = 0;

	pInputLayoutDesc[4].SemanticName = "BINORMAL";
	pInputLayoutDesc[4].SemanticIndex = 0;
	pInputLayoutDesc[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[4].InputSlot = 0;
	pInputLayoutDesc[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[4].InstanceDataStepRate = 0;

	unsigned numElements = ARRAYSIZE(pInputLayoutDesc);

	CubeShader.init(pD3D11Device, hWnd);
	CubeShader.attachVS(L"Cube.vsh", pInputLayoutDesc, numElements);
	CubeShader.attachPS(L"Cube.psh");
	CubeShader.end();
}



void Cube::init_texture(ID3D11Device *pD3D11Device)
{

	HRESULT hr;
	hr = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, L"../../media/textures/stone02.dds", NULL, NULL, &m_pTextures[0], NULL);
	DebugHR(hr);
	hr = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, L"../../media/textures/bump02.dds", NULL, NULL, &m_pTextures[1], NULL);
	DebugHR(hr);
	hr = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, L"../../media/textures/spec02.dds", NULL, NULL, &m_pTextures[2], NULL);
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
}

bool Cube::LoadModel(char *modelFile)
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
	while (ch != ':')
	{
		fin.get(ch);
	}

	// Read in the vertex count.
	fin >> m_VertexCount;
	m_IndexCount = m_VertexCount;

	pModelVertex  = new ModelType[m_VertexCount];
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

	for (int i = 0; i != m_VertexCount; ++i)
	{
		fin >> pModelVertex[i].x >> pModelVertex[i].y >> pModelVertex[i].z;
		fin >> pModelVertex[i].tu >> pModelVertex[i].tv;
		fin >> pModelVertex[i].nx >> pModelVertex[i].ny >> pModelVertex[i].nz;
	}
	fin.close();

	return true;

}

void Cube::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;


	// Calculate the number of faces in the model.
	faceCount = m_VertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (i=0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x  = pModelVertex[index].x;
		vertex1.y  = pModelVertex[index].y;
		vertex1.z  = pModelVertex[index].z;
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
		pModelVertex[index - 1].nx = normal.x;
		pModelVertex[index - 1].ny = normal.y;
		pModelVertex[index - 1].nz = normal.z;
		pModelVertex[index - 1].tx = tangent.x;
		pModelVertex[index - 1].ty = tangent.y;
		pModelVertex[index - 1].tz = tangent.z;
		pModelVertex[index - 1].bx = binormal.x;
		pModelVertex[index - 1].by = binormal.y;
		pModelVertex[index - 1].bz = binormal.z;

		pModelVertex[index - 2].nx = normal.x;
		pModelVertex[index - 2].ny = normal.y;
		pModelVertex[index - 2].nz = normal.z;
		pModelVertex[index - 2].tx = tangent.x;
		pModelVertex[index - 2].ty = tangent.y;
		pModelVertex[index - 2].tz = tangent.z;
		pModelVertex[index - 2].bx = binormal.x;
		pModelVertex[index - 2].by = binormal.y;
		pModelVertex[index - 2].bz = binormal.z;

		pModelVertex[index - 3].nx = normal.x;
		pModelVertex[index - 3].ny = normal.y;
		pModelVertex[index - 3].nz = normal.z;
		pModelVertex[index - 3].tx = tangent.x;
		pModelVertex[index - 3].ty = tangent.y;
		pModelVertex[index - 3].tz = tangent.z;
		pModelVertex[index - 3].bx = binormal.x;
		pModelVertex[index - 3].by = binormal.y;
		pModelVertex[index - 3].bz = binormal.z;
	}

	return;
}

void Cube::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
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

void Cube::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
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
}