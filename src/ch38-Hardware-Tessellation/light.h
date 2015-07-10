
#include "d3d/d3dApp.h"
#include <d3d/d3dShader.h>
#include "d3d/d3dLight.h"

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

	bool LoadModel(char *modelFile);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile, ID3D11ShaderResourceView *m_pTexture);
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

	struct LightBuffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};
	LightBuffer cbLight;

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
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

	int m_VertexCount;
	int m_IndexCount;

	Shader CubeShader;
	std::vector<D3DPointLight> pointLights;
};


void Cube::Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &Model,  
				  const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj)
{

	cbMatrix.model  = XMMatrixTranspose(Model);
	cbMatrix.view   = XMMatrixTranspose(View);
	cbMatrix.proj   = XMMatrixTranspose(Proj);
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbMatrix, 0, 0 );
	pD3D11DeviceContext->DSSetConstantBuffers( 0, 1, &m_pMVPBuffer);

	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex); 
	offset = 0;

	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	CubeShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

}

bool Cube::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	///////////////////////////Index Buffer ////////////////////////////////
	//LoadModel("../../media/objects/square.txt");

	m_VertexCount = 3;
	m_IndexCount  = 3;
	Vertex *VertexData = new Vertex[m_VertexCount];
	unsigned long * IndexData = new unsigned long[m_IndexCount];

	// Load the vertex array with data.
	VertexData[0].Pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	VertexData[1].Pos = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	VertexData[2].Pos = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	
	VertexData[0].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	VertexData[1].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	VertexData[2].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data.
	IndexData[0] = 0;  // Bottom left.
	IndexData[1] = 1;  // Top middle.
	IndexData[2] = 2;  // Bottom right.

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


bool Cube::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc[2];
	pInputLayoutDesc[0].SemanticName = "POSITION";
	pInputLayoutDesc[0].SemanticIndex = 0;
	pInputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[0].InputSlot = 0;
	pInputLayoutDesc[0].AlignedByteOffset = 0;
	pInputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[0].InstanceDataStepRate = 0;

	pInputLayoutDesc[1].SemanticName = "TEXCOORD";
	pInputLayoutDesc[1].SemanticIndex = 0;
	pInputLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pInputLayoutDesc[1].InputSlot = 0;
	pInputLayoutDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[1].InstanceDataStepRate = 0;

	unsigned numElements = ARRAYSIZE(pInputLayoutDesc);

	CubeShader.init(pD3D11Device, hWnd);
	CubeShader.attachVS(L"light.vsh", pInputLayoutDesc, numElements);
	CubeShader.attachHS(L"light.hsh");
	CubeShader.attachDS(L"light.dsh");
	CubeShader.attachPS(L"light.psh");
	CubeShader.end();

	return true;
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
