#ifndef PARTICLE_H
#define PARTICLE_H

#include "d3d/d3dApp.h"
#include "d3d/d3dShader.h"

class Particle
{
public:
	Particle()
	{
		m_pInputLayout        = NULL;
		m_pMVPBuffer          = NULL;
		m_pLightBuffer        = NULL;
		m_pVertexBuffer       = NULL;
		m_pIndexBuffer        = NULL;
		m_pTexture            = NULL;
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMMATRIX &Model,  
		        const XMMATRIX &View, const XMMATRIX &Proj)
	{

		int index, i;
		HRESULT result;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Vertex* m_vertices;
		memset(m_vertices, 0, (sizeof(Vertex) * m_VertexCount));

		// Now build the vertex array from the particle list array.  Each particle is a quad made out of two triangles.
		index = 0;

		for(int i = 0; i < m_currentParticleCount; i++)
		{
			// Bottom left.
			m_vertices[index].Position  = D3DXVECTOR3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord  = D3DXVECTOR2(0.0f, 1.0f);
			m_vertices[index].Color     = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;

			// Top left.
			m_vertices[index].Position = D3DXVECTOR3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
			m_vertices[index].Color    = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;

			// Bottom right.
			m_vertices[index].Position = D3DXVECTOR3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
			m_vertices[index].Color    = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;

			// Bottom right.
			m_vertices[index].Position = D3DXVECTOR3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
			m_vertices[index].Color    = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;

			// Top left.
			m_vertices[index].Position = D3DXVECTOR3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
			m_vertices[index].Color    = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;

			// Top right.
			m_vertices[index].Position = D3DXVECTOR3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
			m_vertices[index].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
			m_vertices[index].Color    = D3DXVECTOR4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
			index++;
		}

		// Lock the vertex buffer.
		result = pD3D11DeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		Vertex *verticesPtr;
		verticesPtr = (Vertex*)mappedResource.pData;
		memcpy(verticesPtr, (void*)m_vertices, (sizeof(Vertex) * m_VertexCount));

		pD3D11DeviceContext->Unmap(m_pVertexBuffer, 0);

		////////////////////////////////////////////////////////////////////////////////
		cbMatrix.model  = XMMatrixTranspose(Model);
		cbMatrix.view   = XMMatrixTranspose(View);
		cbMatrix.proj   = XMMatrixTranspose(Proj);
		pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer, 0, NULL, &cbMatrix, 0, 0 );
		pD3D11DeviceContext->VSSetConstantBuffers( 0, 1, &m_pMVPBuffer);

		unsigned int stride;
		unsigned int offset;
		stride = sizeof(Vertex); 
		offset = 0;
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pD3D11DeviceContext->PSSetShaderResources( 0, 1, &m_pTexture );
		pD3D11DeviceContext->PSSetSamplers( 0, 1, &m_pTexSamplerState );

		ParticleShader.use(pD3D11DeviceContext);
		pD3D11DeviceContext->DrawIndexedInstanced(m_IndexCount, 4, 0, 0, 0);

	}


	void shutdown()
	{
		ReleaseCOM(m_pRenderTargetView  )
		ReleaseCOM(m_pMVPBuffer         )
		ReleaseCOM(m_pLightBuffer       )
		ReleaseCOM(m_pVertexBuffer      )
		ReleaseCOM(m_pIndexBuffer       )
	}

	bool init_buffer (ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	bool init_shader (ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device, LPCWSTR texFile);
	
	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();
	void Frame(float frameTime);

private:

	float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	float m_particleVelocity, m_particleVelocityVariation;
	float m_particleSize, m_particlesPerSecond;
	int   m_maxParticles;
	int   m_currentParticleCount;
	float m_accumulatedTime;
	struct ParticleType
	{
		float positionX, positionY, positionZ;
		float red, green, blue;
		float velocity;
		bool active;
	};
	ParticleType *m_particleList;

	struct CameraBuffer
	{
		D3DXVECTOR3 camPos;
		float padding;
	};

	struct MatrixBuffer
	{
		XMMATRIX  model;
		XMMATRIX  view;
		XMMATRIX  proj;

	};
	MatrixBuffer cbMatrix;


	struct  Vertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 TexCoord;
		D3DXVECTOR4 Color;
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

	Shader ParticleShader;
};

#endif