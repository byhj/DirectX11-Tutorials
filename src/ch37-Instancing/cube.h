
#include "d3d/d3dApp.h"
#include <d3d/d3dShader.h>

class Cube
{
public:
	Cube()
	{
		m_pInputLayout        = NULL;
		m_pMVPBuffer          = NULL;
		m_pLightBuffer        = NULL;
		m_pVertexBuffer       = NULL;
		m_pIndexBuffer        = NULL;
		m_pTexture            = NULL;
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &Model,  
		                             const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj);

	void shutdown()
	{
			ReleaseCOM(m_pRenderTargetView  )
			ReleaseCOM(m_pMVPBuffer         )
			ReleaseCOM(m_pLightBuffer       )
			ReleaseCOM(m_pVertexBuffer      )
			ReleaseCOM(m_pIndexBuffer       )
	}

	bool load_model(char *modelFile);
	bool load_obj(char *objFile);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile);

private:
	struct CameraBuffer
	{
		XMFLOAT3 camPos;
		float padding;
	};

	struct MatrixBuffer
	{
		XMMATRIX  model;
		XMMATRIX  view;
		XMMATRIX  proj;

	};
	MatrixBuffer cbMatrix;

	struct InstanceType
	{
		XMFLOAT4 position;
	};

	struct LightBuffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};
	LightBuffer cbLight;

	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
		XMFLOAT3 Normal;
	};

	struct ModelVertex
	{
		float x, y , z;
		float u, v;
		float nx, ny, nz;
	};
	ModelVertex  *m_pModelVertex;

	ID3D11RenderTargetView   *m_pRenderTargetView;
	ID3D11Buffer             *m_pMVPBuffer;
	ID3D11Buffer             *m_pLightBuffer;
	ID3D11Buffer             *m_CameraBuffer;
	ID3D11Buffer             *m_pVertexBuffer;
	ID3D11Buffer             *m_pIndexBuffer;
	ID3D11ShaderResourceView *m_pTexture;
	ID3D11SamplerState       *m_pTexSamplerState;
	ID3D11InputLayout        *m_pInputLayout;
	ID3D11Buffer *m_pInstanceBuffer;

	int m_VertexCount;
	int m_IndexCount;
	std::vector<Vertex> VertexData;
	std::vector<unsigned int> IndexData;

	Shader CubeShader;
};


void Cube::Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &Model,  
				  const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj)
{

	cbMatrix.model  = XMMatrixTranspose(Model);
	cbMatrix.view   = XMMatrixTranspose(View);
	cbMatrix.proj   = XMMatrixTranspose(Proj);
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbMatrix, 0, 0 );
	pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, &m_pMVPBuffer);

	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];
	// Set the buffer strides.
	strides[0] = sizeof(Vertex); 
	strides[1] = sizeof(InstanceType); 

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = m_pVertexBuffer;	
	bufferPointers[1] = m_pInstanceBuffer;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pD3D11DeviceContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pD3D11DeviceContext->PSSetShaderResources( 0, 1, &m_pTexture );
	pD3D11DeviceContext->PSSetSamplers( 0, 1, &m_pTexSamplerState );

	CubeShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexedInstanced(m_IndexCount, 4, 0, 0, 0);

}

bool Cube::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	///////////////////////////Index Buffer ////////////////////////////////
	load_obj("../../media/objects/cube.obj");

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
	VBO.pSysMem          = &VertexData[0];
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
	IBO.pSysMem          = &IndexData[0];
	IBO.SysMemPitch      = 0;
	IBO.SysMemSlicePitch = 0;

	hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
	DebugHR(hr);


	////////////////////////////////MVP Buffer//////////////////////////////////////

	D3D11_BUFFER_DESC mvpBufferDesc;	
	ZeroMemory(&mvpBufferDesc, sizeof(D3D11_BUFFER_DESC));
	mvpBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpBufferDesc.ByteWidth      = sizeof(MatrixBuffer);
	mvpBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpBufferDesc.CPUAccessFlags = 0;
	mvpBufferDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);
	DebugHR(hr);

	InstanceType *InstanceData;
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA IntData;
	int InstanceCount  = 4;
	InstanceData = new InstanceType[InstanceCount];
	if (!InstanceData)
	{
		return false;
	}
	// Load the instance array with data.
	InstanceData[0].position = XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f);
	InstanceData[1].position = XMFLOAT4(-1.0f,  1.0f, 1.0f, 1.0f);
	InstanceData[2].position = XMFLOAT4( 1.0f, -1.0f, 1.0f, 1.0f);
	InstanceData[3].position = XMFLOAT4( 1.0f,  1.0f, 1.0f, 1.0f);
	// Set up the description of the instance buffer.

	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * InstanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	IntData.pSysMem = InstanceData;
	IntData.SysMemPitch = 0;
	IntData.SysMemSlicePitch = 0;
	// Create the instance buffer.
	hr = pD3D11Device->CreateBuffer(&instanceBufferDesc, &IntData, &m_pInstanceBuffer);
    DebugHR(hr);

	// Release the instance array now that the instance buffer has been created and loaded.
	delete [] InstanceData;
	InstanceData = 0;
	return true;
}


bool Cube::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc[4];
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

	pInputLayoutDesc[3].SemanticName         = "TEXCOORD";
	pInputLayoutDesc[3].SemanticIndex        = 1;
	pInputLayoutDesc[3].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[3].InputSlot            = 1;
	pInputLayoutDesc[3].AlignedByteOffset    = 0;
	pInputLayoutDesc[3].InputSlotClass       = D3D11_INPUT_PER_INSTANCE_DATA;
	pInputLayoutDesc[3].InstanceDataStepRate = 1;

	unsigned numElements = ARRAYSIZE(pInputLayoutDesc);

	CubeShader.init(pD3D11Device, hWnd);
	CubeShader.attachVS(L"cube.vsh", pInputLayoutDesc, numElements);
	CubeShader.attachPS(L"cube.psh");
	CubeShader.end();

	return true;
}

void Cube::init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile)
{
	HRESULT hr;
	hr = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, texFile, NULL,NULL, &m_pTexture, NULL);
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

bool Cube::load_model(char *modelFile)
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
	fin >> m_VertexCount;
	m_IndexCount = m_VertexCount;

	m_pModelVertex  = new ModelVertex[m_VertexCount];
	if (!m_pModelVertex)
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
		fin >> m_pModelVertex[i].x  >> m_pModelVertex[i].y >> m_pModelVertex[i].z;
		fin >> m_pModelVertex[i].u  >> m_pModelVertex[i].v;
		fin >> m_pModelVertex[i].nx >> m_pModelVertex[i].ny >> m_pModelVertex[i].nz;
	}
	fin.close();

	return true;
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
	}//While
	fin.close();

	for (int i = 0; i != vPosIndex.size(); ++i)
	{
		vt.Position = vPos[vPosIndex[i] - 1];
		vt.TexCoord = vTex[vTexIndex[i] - 1];
		vt.Normal   = vNormal[vNormalIndex[i] - 1];
		VertexData.push_back(vt);
		IndexData.push_back(i);
	}
	m_VertexCount = VertexData.size();
	m_IndexCount = IndexData.size();

	return true;
}
