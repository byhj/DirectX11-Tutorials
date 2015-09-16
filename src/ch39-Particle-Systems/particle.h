#ifndef PARTICLE_H
#define PARTICLE_H

#include "d3d/App.h"
#include "d3d/Shader.h"

namespace byhj
{


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
		XMFLOAT3 camPos;
		float padding;
	};

	struct d3d::MatrixBuffer
	{
		XMFLOAT4X4  model;
		XMFLOAT4X4  view;
		XMFLOAT4X4  proj;

	};
	d3d::MatrixBuffer cbMatrix;


	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
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
	ID3D11Buffer *m_pInstanceBuffer;

	int m_VertexCount;
	int m_IndexCount;
    Vertex* m_vertices;
	d3d::Shader ParticleShader;
};

}

#endif