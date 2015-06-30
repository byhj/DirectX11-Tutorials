
#include "d3d/d3dApp.h"
#include <d3d/d3dShader.h>
#include "d3d/d3dLight.h"

class Object
{
public:
	Object()
	{
		m_pInputLayout        = NULL;
		m_pMVPBuffer          = NULL;
		m_pVertexBuffer       = NULL;
		m_pIndexBuffer        = NULL;
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMMATRIX &Model,  
		                             const XMMATRIX &View, const XMMATRIX &Proj);

	void shutdown()
	{
			ReleaseCOM(m_pRenderTargetView  )
			ReleaseCOM(m_pMVPBuffer         )
			ReleaseCOM(m_pVertexBuffer      )
			ReleaseCOM(m_pIndexBuffer       )
	}

	bool LoadModel(char *modelFile);
	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile, ID3D11ShaderResourceView *m_pTexture);
private:

	struct MatrixBuffer
	{
		XMMATRIX  model;
		XMMATRIX  view;
		XMMATRIX  proj;

	};
	MatrixBuffer cbMatrix;

	struct DistortionBuffer
	{    
		XMFLOAT2 distortion1;
		XMFLOAT2 distortion2;
		XMFLOAT2 distortion3;
		float  distortionScale;
		float  distortionBias;
	};
	DistortionBuffer cbDistortion;

	struct NoiseBuffer
	{
		float   frameTime;
		XMFLOAT3 scrollSpeeds;
		XMFLOAT3 scales;
		float    padding;
	};
	NoiseBuffer cbNoise;

	struct Vertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 TexCoord;
		D3DXVECTOR3 Normal;
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
	ID3D11Buffer             *m_pNoiseBuffer;
	ID3D11Buffer             *m_pDistortBuffer;
	ID3D11Buffer             *m_pVertexBuffer;
	ID3D11Buffer             *m_pIndexBuffer;
	ID3D11ShaderResourceView *m_pTextures[3];
	ID3D11SamplerState       *m_pTexSamplerState;
	ID3D11SamplerState       *m_pTexSamplerState1;
	ID3D11InputLayout        *m_pInputLayout;

	int m_VertexCount;
	int m_IndexCount;

	Shader ObjectShader;
	std::vector<D3DPointLight> pointLights;
};


void Object::Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMMATRIX &Model,  
				  const XMMATRIX &View, const XMMATRIX &Proj)
{

	cbMatrix.model  = XMMatrixTranspose(Model);
	cbMatrix.view   = XMMatrixTranspose(View);
	cbMatrix.proj   = XMMatrixTranspose(Proj);
	pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbMatrix, 0, 0 );
	pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, &m_pMVPBuffer);


	//////////////////////////////////////////////////////////////////////////////
	static float frameTime = 0.0f;
	// Increment the frame time counter.
	frameTime += 0.001f;
	if(frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}
	cbNoise.frameTime = frameTime;
	cbNoise.scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);
	cbNoise.scales       = XMFLOAT3(1.0f, 2.0f, 3.0f);
	cbNoise.padding = 0.0f;
	pD3D11DeviceContext->UpdateSubresource(m_pNoiseBuffer, 0, NULL, &cbNoise, 0, 0 );
	pD3D11DeviceContext->VSSetConstantBuffers(1, 1, &m_pNoiseBuffer);

	///////////////////////////////////////////////////////////////////////////////
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(Vertex); 
	offset = 0;

	pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pD3D11DeviceContext->PSSetShaderResources(0, 3, m_pTextures);  
	pD3D11DeviceContext->PSSetSamplers( 0, 1, &m_pTexSamplerState );
	pD3D11DeviceContext->PSSetSamplers( 1, 1, &m_pTexSamplerState1 );
	ObjectShader.use(pD3D11DeviceContext);
	pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);

}

bool Object::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	HRESULT hr;

	///////////////////////////Index Buffer ////////////////////////////////
	LoadModel("../../media/objects/square.txt");


	Vertex *VertexData = new Vertex[m_VertexCount];
	unsigned long * IndexData = new unsigned long[m_IndexCount];

	for (int i = 0; i != m_VertexCount; ++i)
	{
		VertexData[i].Position = D3DXVECTOR3(m_pModelVertex[i].x,  m_pModelVertex[i].y,  m_pModelVertex[i].z);
		VertexData[i].TexCoord = D3DXVECTOR2(m_pModelVertex[i].u,  m_pModelVertex[i].v);
		VertexData[i].Normal   = D3DXVECTOR3(m_pModelVertex[i].nx, m_pModelVertex[i].ny, m_pModelVertex[i].nz);
		IndexData[i] = i;
	}

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

	///////////////////////////////////////Light buffer////////////////////////////////////////

	D3D11_BUFFER_DESC noiseBufferDesc;	
	ZeroMemory(&noiseBufferDesc, sizeof(D3D11_BUFFER_DESC));
	noiseBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	noiseBufferDesc.ByteWidth      = sizeof(NoiseBuffer);
	noiseBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	noiseBufferDesc.CPUAccessFlags = 0;
	noiseBufferDesc.MiscFlags      = 0;

	hr = pD3D11Device->CreateBuffer(&noiseBufferDesc, NULL, &m_pNoiseBuffer);

	//////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC distortBufferDesc;	
	ZeroMemory(&distortBufferDesc, sizeof(D3D11_BUFFER_DESC));
	distortBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	distortBufferDesc.ByteWidth      = sizeof(DistortionBuffer);
	distortBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	distortBufferDesc.CPUAccessFlags = 0;
	distortBufferDesc.MiscFlags      = 0;

	cbDistortion.distortion1 = XMFLOAT2(0.1f, 0.2f);
	cbDistortion.distortion2 = XMFLOAT2(0.1f, 0.3f);
	cbDistortion.distortion3 = XMFLOAT2(0.1f, 0.1f);
	cbDistortion.distortionScale = 0.8f;
	cbDistortion.distortionBias  = 0.5f;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA distortSub;
	distortSub.pSysMem          = &cbDistortion;
	distortSub.SysMemPitch      = 0;
	distortSub.SysMemSlicePitch = 0;
	hr = pD3D11Device->CreateBuffer(&distortBufferDesc, &distortSub, &m_pDistortBuffer);
	DebugHR(hr);
	int lightSlot = 0;
	pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, &m_pNoiseBuffer);


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


	D3D11_SAMPLER_DESC samplerDesc2;
	samplerDesc2.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc2.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc2.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc2.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc2.MipLODBias = 0.0f;
	samplerDesc2.MaxAnisotropy = 1;
	samplerDesc2.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc2.BorderColor[0] = 0;
	samplerDesc2.BorderColor[1] = 0;
	samplerDesc2.BorderColor[2] = 0;
	samplerDesc2.BorderColor[3] = 0;
	samplerDesc2.MinLOD = 0;
	samplerDesc2.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pD3D11Device->CreateSamplerState(&samplerDesc2, &m_pTexSamplerState1);

	hr = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, L"../../media/textures/fire01.dds", NULL,NULL, &m_pTextures[0], NULL);
	DebugHR(hr);

	hr = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, L"../../media/textures/noise01.dds", NULL,NULL, &m_pTextures[1], NULL);
	DebugHR(hr);
	hr = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, L"../../media/textures/alpha01.dds", NULL,NULL, &m_pTextures[2], NULL);
	DebugHR(hr);
	return true;
}


bool Object::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc[3];
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

	unsigned numElements = ARRAYSIZE(pInputLayoutDesc);

	ObjectShader.init(pD3D11Device, hWnd);
	ObjectShader.attachVS(L"light.vsh", pInputLayoutDesc, numElements);
	ObjectShader.attachPS(L"light.psh");
	ObjectShader.end();

	return true;
}

bool Object::LoadModel(char *modelFile)
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
