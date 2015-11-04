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
	void Render(ID3D11DeviceContext *pD3D11DeviceContext, const d3d::MatrixBuffer &matrix);
	void Shutdown();

private:
	void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
	void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
	void init_texture(ID3D11Device *pD3D11Device);
	void load_model(char *modelFile);

	struct  Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
		XMFLOAT3 Normal;
	};
	struct InstanceType
	{
		XMFLOAT4 position;
	};

	d3d::MatrixBuffer cbMatrix;
	ComPtr<ID3D11InputLayout> m_pInputLayout;
	ID3D11VertexShader       *m_pVS               = nullptr;
	ID3D11PixelShader        *m_pPS               = nullptr;
	ComPtr<ID3D11Buffer> m_pMVPBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBufferr;
	ComPtr<ID3D11Buffer> m_pVertexBufferr;
	ID3D11Buffer             *m_pInstanceBuffer   = nullptr;
	ComPtr<ID3D11ShaderResourceView> m_pTexture;
	ComPtr<ID3D11SamplerState> m_pTexSamplerState;
	d3d::Shader CubeShader;

	std::vector<Vertex> m_VertexData;
	std::vector<DWORD> m_IndexData;
};


}


#endif 