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

void Cube::Update()
{

}
void Cube::Render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix)
{


	cbMatrix.model = matrix.model;
	cbMatrix.view  = matrix.view;
	cbMatrix.proj  = matrix.proj;
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbMatrix, 0, 0);
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &m_pMVPBuffer);
	pD3D11DeviceContext->PSSetShaderResources(0, 1, &m_pTexture);
	pD3D11DeviceContext->PSSetSamplers(0, 1, &m_pTexSamplerState);

	CubeShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexed(m_VertexData.size(), 0, 0);


}

void Cube::Shutdown()
{
	ReleaseCOM(m_pInputLayout)
	ReleaseCOM(m_pMVPBuffer)
	ReleaseCOM(m_pVertexBuffer)
	ReleaseCOM(m_pIndexBuffer)
	ReleaseCOM(m_pLightBuffer)
	ReleaseCOM(m_pTexSamplerState)
	ReleaseCOM(m_pTexture)

}

void Cube::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;
	load_obj("../../media/objects/cube.obj");

	///////////////////////////Index Buffer ////////////////////////////////

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth           = sizeof(Vertex)* m_VertexData.size();
	VertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags      = 0;
	VertexBufferDesc.MiscFlags           = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA VBO;
	VBO.pSysMem          = &m_VertexData[0];
	VBO.SysMemPitch      = 0;
	VBO.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	hr = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
	DebugHR(hr);

	/////////////////////////////////Index Buffer ///////////////////////////////////////

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth           = sizeof(DWORD)* m_IndexData.size();
	IndexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags      = 0;
	IndexBufferDesc.MiscFlags           = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA IBO;
	IBO.pSysMem          = &m_IndexData[0];
	IBO.SysMemPitch      = 0;
	IBO.SysMemSlicePitch = 0;

	hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
	DebugHR(hr);

	////////////////////////////////////////////////////////////////////////////////////////

	// Set vertex buffer stride and offset.=
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex);
	offset = 0;
	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

	dataPtr2->diffuseColor   = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	dataPtr2->lightDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	dataPtr2->padding = 0.0f;

	pD3D11DeviceContext->Unmap(m_pLightBuffer, 0);

	int lightSlot = 0;
	pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, &m_pLightBuffer);


}

void Cube::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
	D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc[3];
	pInputLayoutDesc[0].SemanticName         = "POSITION";
	pInputLayoutDesc[0].SemanticIndex        = 0;
	pInputLayoutDesc[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[0].InputSlot            = 0;
	pInputLayoutDesc[0].AlignedByteOffset    = 0;
	pInputLayoutDesc[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
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

	CubeShader.init(pD3D11Device, hWnd);
	CubeShader.attachVS(L"Cube.vsh", pInputLayoutDesc, numElements);
	CubeShader.attachPS(L"Cube.psh");
	CubeShader.end();
}



void Cube::init_texture(ID3D11Device *pD3D11Device)
{

	HRESULT hr;
	hr = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, L"../../media/textures/stone.dds", NULL, NULL, &m_pTexture, NULL);
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

void Cube::load_model(char *modelFile)
{
	std::ifstream fin;
	char ch;
	int i;
	fin.open(modelFile);

	if (fin.fail())
		return;

	// Read up to the value of vertex count.
	fin.get(ch);
	while (ch != ':')
	{
		fin.get(ch);
	}

	int VertexCount, IndexCount;
	// Read in the vertex count.
	fin >> VertexCount;
	IndexCount = VertexCount;
	m_VertexData.resize(VertexCount);
	m_IndexData.resize(IndexCount);


	//Read up the beginning of the data
	fin.get(ch);
	while (ch != ':')
	{
		fin.get(ch);
	}

	for (int i = 0; i != VertexCount; ++i)
	{
		fin >> m_VertexData[i].Position.x >> m_VertexData[i].Position.y >> m_VertexData[i].Position.z;
		fin >> m_VertexData[i].TexCoord.x >> m_VertexData[i].TexCoord.y;
		fin >> m_VertexData[i].Normal.x   >> m_VertexData[i].Normal.y   >> m_VertexData[i].Normal.z;

		m_IndexData[i] = static_cast<DWORD>(i);
	}
	fin.close();


}


bool Cube::load_obj(char *objFile)
{
	std::ifstream fin;
	char ch;
	fin.open(objFile);

	if (fin.fail())
	{
		MessageBox(NULL, L"Can not open the Model File", L"Error", MB_OK | MB_ICONERROR);
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(ch);
	Vertex vt;
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
	XMFLOAT3 Normal;
	std::vector<XMFLOAT3> vPos;
	std::vector<XMFLOAT2> vTex;
	std::vector<XMFLOAT3> vNormal;
	std::vector<unsigned int> vPosIndex;
	std::vector<unsigned int> vTexIndex;
	std::vector<unsigned int> vNormalIndex;
	unsigned int a[3], b[3], c[3];
	char ct;

	while (!fin.eof())
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
				//After convert, Cube mode is CCW, so is not show!!
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
		while (ch != '\n')
		{
			fin.get(ch);
		}

		// Start reading the beginning of the next line.
		fin.get(ch);
	}//While
	fin.close();

	for (int i = 0; i != vPosIndex.size(); ++i)
	{
		vt.Position = vPos[vPosIndex[i] - 1];
		vt.TexCoord = vTex[vTexIndex[i] - 1];
		vt.Normal   = vNormal[vNormalIndex[i] - 1];
		m_VertexData.push_back(vt);
		m_IndexData.push_back(i);
	}

	return true;
}

}