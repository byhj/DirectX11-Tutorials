#include "particle.h"

namespace byhj
{
void Particle::Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &Model,
		              const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj)
	{

		int index, i;
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		// Now build the vertex array from the particle list array.  Each particle is a quad made out of two triangles.
		index = 0;

		for (int i = 0; i < m_currentParticleCount; i++)
		{
			// Bottom left.
			m_vertices[index].Position  = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord  = XMFLOAT2(0.0f, 1.0f);
			m_vertices[index].Color     = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;

			// Top left.
			m_vertices[index].Position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord = XMFLOAT2(0.0f, 0.0f);
			m_vertices[index].Color    = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;

			// Bottom right.
			m_vertices[index].Position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord = XMFLOAT2(1.0f, 1.0f);
			m_vertices[index].Color    = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;

			// Bottom right.
			m_vertices[index].Position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord = XMFLOAT2(1.0f, 1.0f);
			m_vertices[index].Color    = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;

			// Top left.
			m_vertices[index].Position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord = XMFLOAT2(0.0f, 0.0f);
			m_vertices[index].Color    = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;

			// Top right.
			m_vertices[index].Position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord = XMFLOAT2(1.0f, 0.0f);
			m_vertices[index].Color    = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;
		}

		// Lock the vertex buffer.
		result = pD3D11DeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		Vertex *vertexPtr = (Vertex*)mappedResource.pData;
		memcpy(vertexPtr, m_vertices, m_VertexCount);

		pD3D11DeviceContext->Unmap(m_pVertexBuffer, 0);

		////////////////////////////////////////////////////////////////////////////////
		cbMatrix.model = Model;
		cbMatrix.view = View;
		cbMatrix.proj = Proj;
		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf() );

		unsigned int stride;
		unsigned int offset;
		stride = sizeof(Vertex);
		offset = 0;
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
		pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0); 
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
		pD3D11DeviceContext->PSSetSamplers(0, 1, m_pTexSamplerState.GetAddressOf());

		ParticleShader.use(pD3D11DeviceContext);
		pD3D11DeviceContext->DrawIndexedInstanced(m_IndexCount, 4, 0, 0, 0);

	}

bool Particle::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
{
	// Set the random deviation of where the particles can be located when emitted.
	m_particleDeviationX = 0.5f;
	m_particleDeviationY = 2.0f;
	m_particleDeviationZ = 2.0f;

	// Set the speed and speed variation of particles.
	m_particleVelocity = 1.0f;
	m_particleVelocityVariation = 0.2f;

	// Set the physical size of the particles.
	m_particleSize = 0.2f;

	// Set the number of particles to emit per second.
	m_particlesPerSecond = 250.0f;

	// Set the maximum number of particles allowed in the particle system.
	m_maxParticles = 5000;

	// Create the particle list.
	m_particleList = new ParticleType[m_maxParticles];
	if(!m_particleList)
	{
		return false;
	}

	// Initialize the particle list.
	for(int i = 0; i < m_maxParticles; i++)
	{
		m_particleList[i].active = false;
	}

	// Initialize the current particle count to zero since none are emitted yet.
	m_currentParticleCount = 0;

	// Clear the initial accumulated time for the particle per second emission rate.
	m_accumulatedTime = 0.0f;


	HRESULT hr;

	////	// Set the maximum number of vertices in the vertex array.
	m_VertexCount = m_maxParticles * 6;

	// Set the maximum number of indices in the index array.
	m_IndexCount = m_VertexCount;
	///////////////////////Index Buffer ////////////////////////////////

	m_vertices = new Vertex[m_VertexCount];
	memset(m_vertices, 0, (sizeof(Vertex) * m_VertexCount));

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth           = sizeof(Vertex) * m_VertexCount;
	VertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags           = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VBO;
	VBO.pSysMem = m_vertices;
	VBO.SysMemPitch = 0;
	VBO.SysMemSlicePitch = 0;
	hr = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
	//DebugHR(hr);

	/////////////////////////////////Index Buffer ///////////////////////////////////////
	unsigned long  *indices = new unsigned long[m_IndexCount];
	// Initialize the index array.
	for(int i=0; i<m_IndexCount; i++)
	{
		indices[i] = i;
	}
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
	IBO.pSysMem          = indices;
	IBO.SysMemPitch      = 0;
	IBO.SysMemSlicePitch = 0;

	hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
	//DebugHR(hr);

	delete [] indices;
	indices = 0;

	////////////////////////////////MVP Buffer//////////////////////////////////////

	D3D11_BUFFER_DESC mvpBufferDesc;	
	ZeroMemory(&mvpBufferDesc, sizeof(D3D11_BUFFER_DESC));
	mvpBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	mvpBufferDesc.ByteWidth      = sizeof(d3d::MatrixBuffer);
	mvpBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	mvpBufferDesc.CPUAccessFlags = 0;
	mvpBufferDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&mvpBufferDesc, NULL, &m_pMVPBuffer);
	//DebugHR(hr);

	return true;
}


bool Particle::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
	HRESULT result;

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

	pInputLayoutDesc[2].SemanticName         = "TEXCOORD";
	pInputLayoutDesc[2].SemanticIndex        = 1;
	pInputLayoutDesc[2].Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pInputLayoutDesc[2].InputSlot            = 0;
	pInputLayoutDesc[2].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
	pInputLayoutDesc[2].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[2].InstanceDataStepRate = 0;

	

	ParticleShader.init(pD3D11Device, vInputLayoutDesc);
	ParticleShader.attachVS(L"Particle.vsh", "VS", "vs_5_0");
	ParticleShader.attachPS(L"Particle.psh");
	ParticleShader.end();

	return true;
}

void Particle::init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile)
{
	HRESULT hr;
	hr = CreateDDSTextureFromFile(pD3D11Device, texFile, NULL,NULL, &m_pTexture, NULL);
	//DebugHR(hr);

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
	//DebugHR(hr);

}
void Particle::Frame(float frameTime)
{
	// Release old particles.
	KillParticles();

	// Emit new particles.
	EmitParticles(frameTime);

	// Update the position of the particles.
	UpdateParticles(frameTime);

}

void Particle::KillParticles()
{
	int i, j;


	// Kill all the particles that have gone below a certain height range.
	for (i=0; i < m_maxParticles; i++)
	{
		if ((m_particleList[i].active == true) && (m_particleList[i].positionY < -5.0f))
		{
			m_particleList[i].active = false;
			m_currentParticleCount--;

			// Now shift all the live particles back up the array to erase the destroyed particle and keep the array sorted correctly.
			for (j=i; j < m_maxParticles - 1; j++)
			{
				m_particleList[j].positionX = m_particleList[j + 1].positionX;
				m_particleList[j].positionY = m_particleList[j + 1].positionY;
				m_particleList[j].positionZ = m_particleList[j + 1].positionZ;
				m_particleList[j].red       = m_particleList[j + 1].red;
				m_particleList[j].green     = m_particleList[j + 1].green;
				m_particleList[j].blue      = m_particleList[j + 1].blue;
				m_particleList[j].velocity  = m_particleList[j + 1].velocity;
				m_particleList[j].active    = m_particleList[j + 1].active;
			}
		}
	}

}
void Particle::EmitParticles(float frameTime)
{
	bool emitParticle, found;
	float positionX, positionY, positionZ, velocity, red, green, blue;
	int index, i, j;


	// Increment the frame time.
	m_accumulatedTime += frameTime;

	// Set emit particle to false for now.
	emitParticle = false;

	// Check if it is time to emit a new particle or not.
	if(m_accumulatedTime  > (1.0f / m_particlesPerSecond))
	{
		m_accumulatedTime = 0.0f;
		emitParticle = true;
	}

	//std::cout << m_accumulatedTime << std::endl;
	// If there are particles to emit then emit one per frame.
	if((emitParticle == true) && (m_currentParticleCount < (m_maxParticles - 1)))
	{
		m_currentParticleCount++;

		// Now generate the randomized particle properties.
		positionX = (((float)rand()-(float)rand())/RAND_MAX) * m_particleDeviationX;
		positionY = (((float)rand()-(float)rand())/RAND_MAX) * m_particleDeviationY;
		positionZ = (((float)rand()-(float)rand())/RAND_MAX) * m_particleDeviationZ;

		velocity = m_particleVelocity + (((float)rand()-(float)rand())/RAND_MAX) * m_particleVelocityVariation;

		red   = (((float)rand()-(float)rand())/RAND_MAX) + 0.5f;
		green = (((float)rand()-(float)rand())/RAND_MAX) + 0.5f;
		blue  = (((float)rand()-(float)rand())/RAND_MAX) + 0.5f;

		// Now since the particles need to be rendered from back to front for blending we have to sort the particle array.
		// We will sort using Z depth so we need to find where in the list the particle should be inserted.
		index = 0;
		found = false;
		while(!found)
		{
			if((m_particleList[index].active == false) || (m_particleList[index].positionZ < positionZ))
			{
				found = true;
			}
			else
			{
				index++;
			}
		}

		// Now that we know the location to insert into we need to copy the array over by one position from the index to make room for the new particle.
		i = m_currentParticleCount;
		j = i - 1;

		while(i != index)
		{
			m_particleList[i].positionX = m_particleList[j].positionX;
			m_particleList[i].positionY = m_particleList[j].positionY;
			m_particleList[i].positionZ = m_particleList[j].positionZ;
			m_particleList[i].red       = m_particleList[j].red;
			m_particleList[i].green     = m_particleList[j].green;
			m_particleList[i].blue      = m_particleList[j].blue;
			m_particleList[i].velocity  = m_particleList[j].velocity;
			m_particleList[i].active    = m_particleList[j].active;
			i--;
			j--;
		}

		// Now insert it into the particle array in the correct depth order.
		m_particleList[index].positionX = positionX;
		m_particleList[index].positionY = positionY;
		m_particleList[index].positionZ = positionZ;
		m_particleList[index].red       = red;
		m_particleList[index].green     = green;
		m_particleList[index].blue      = blue;
		m_particleList[index].velocity  = velocity;
		m_particleList[index].active    = true;
	}

}

void Particle::UpdateParticles(float frameTime)
{
	// Each frame we update all the particles by making them move downwards using their position, velocity, and the frame time.
	for(int i = 0; i < m_currentParticleCount; i++)
		m_particleList[i].positionY = m_particleList[i].positionY - (m_particleList[i].velocity * frameTime * 0.001f);

	return;
}




}
