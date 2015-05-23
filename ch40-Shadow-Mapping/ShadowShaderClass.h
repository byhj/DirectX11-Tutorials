#ifndef  SHADERCLASS_H
#define  SHADERCLASS_H

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>


class  ShadowShaderClass
{
public:
	 ShadowShaderClass();
	 ShadowShaderClass(const ShadowShaderClass &);
	~ ShadowShaderClass();

	bool Init(ID3D11Device *, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, 
		ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4);

private:
    bool InitShader(ID3D11Device *, HWND, WCHAR *, WCHAR * );
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob *, HWND, WCHAR *);
	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, 
		ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4);
	void RenderShader(ID3D11DeviceContext*, int);

	struct MatrixBuffer
	{
		D3DXMATRIX World;
		D3DXMATRIX View;
		D3DXMATRIX Proj;
		D3DXMATRIX lightView;
		D3DXMATRIX lightProjection;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
	};

	struct LightBufferType2
	{
		D3DXVECTOR3 lightPosition;
		float padding;
	};

private:
	ID3D11VertexShader *pVS_Shader;
	ID3D11PixelShader  *pPS_Shader;
	ID3D11InputLayout  *pInputLayout;
	ID3D11Buffer       *pMatrixBuffer;
	ID3D11SamplerState* m_sampleStateWrap;
	ID3D11SamplerState* m_sampleStateClamp;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_lightBuffer2;
	WCHAR *pVS_File;
	WCHAR *pPS_File;
};


#endif // ! SHADERCLASS_H
