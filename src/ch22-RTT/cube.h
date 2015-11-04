#ifndef Cube_H
#define Cube_H

#include <windows.h>
#include <xnamath.h>
#include <vector>

#include "d3d/Shader.h"
#include "d3d/Utility.h"


namespace byhj
{


class Cube
{
public:
	Cube();
	~Cube();

	void Init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
	void Update();
	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix);
	void Shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device);

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
		float x, y, z;
		float u, v;
		float nx, ny, nz;
	};
	ModelVertex  *m_pModelVertex;
	int m_VertexCount;
	int m_IndexCount;
	ModelType  *pModelVertex;

	bool LoadModel(char *modelFile);
	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	void CalculateNormal(VectorType, VectorType, VectorType&);

	struct LightBuffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};
	LightBuffer cbLight;

	struct CameraBuffer
	{
		XMFLOAT3 camPos;
		float padding;
	};

	d3d::MatrixBuffer cbMatrix;
	ComPtr<ID3D11InputLayout> m_pInputLayout;
	ID3D11VertexShader       *m_pVS               = nullptr;
	ID3D11PixelShader        *m_pPS               = nullptr;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ID3D11Buffer             *m_pLightBuffer      = nullptr;
	ID3D11Buffer             *m_CameraBuffer      = nullptr;
	ID3D11ShaderResourceView *m_pTextures[2];
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	d3d::Shader CubeShader;
};


}


#endif 