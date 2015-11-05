#ifndef CUBE_H
#define CUBE_H

#include "d3d/Shader.h"
#include "d3d/Utility.h"
#include "DirectXTK/DDSTextureLoader.h"


namespace byhj
{

class Cube
{
public:
	Cube()
	{
		m_pInputLayout        = NULL;
		m_pMVPBuffer          = NULL;
		pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, m_pLightBuffer.GetAddressOf());        = NULL;
		m_pVertexBuffer       = NULL;
		m_pIndexBuffer        = NULL;
		m_pTextures[0]        = NULL;
		m_pTextures[1]        = NULL;
	}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const XMFLOAT4X4 &Model,  
		                             const XMFLOAT4X4 &View, const XMFLOAT4X4 &Proj);

	void shutdown()
	{
			ReleaseCOM(m_pRenderTargetView  )
			ReleaseCOM(m_pMVPBuffer         )
			ReleaseCOM(pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, m_pLightBuffer.GetAddressOf());       )
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

	d3d::MatrixBuffer cbMatrix;

	struct LightBuffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};
	LightBuffer cbLight;

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};
	struct ModelVertex
	{
		float x, y , z;
		float u, v;
		float nx, ny, nz;
	};
	ModelVertex  *m_pModelVertex;

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ID3D11Buffer             *pD3D11DeviceContext->PSSetConstantBuffers(lightSlot, 1, m_pLightBuffer.GetAddressOf());;
	ComPtr<ID3D11Buffer> m_CameraBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ID3D11ShaderResourceView *m_pTextures[2];
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	ComPtr<ID3D11InputLayout> m_pInputLayout;

	int m_VertexCount;
	int m_IndexCount;
	ModelType  *pModelVertex;
	d3d::Shader CubeShader;

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	void CalculateNormal(VectorType, VectorType, VectorType&);

};

}
#endif