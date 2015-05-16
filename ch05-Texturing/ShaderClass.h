#ifndef  SHADERCLASS_H
#define  SHADERCLASS_H

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>


class  ShaderClass
{
public:
	 ShaderClass();
	 ShaderClass(const ShaderClass &);
	~ ShaderClass();

	bool Init(ID3D11Device *, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

private:
    bool InitShader(ID3D11Device *, HWND, WCHAR *, WCHAR * );
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob *, HWND, WCHAR *);
	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

	struct MatrixBuffer
	{
		D3DXMATRIX World;
		D3DXMATRIX View;
		D3DXMATRIX Proj;
	};

private:
	ID3D11VertexShader *pVS_Shader;
	ID3D11PixelShader  *pPS_Shader;
	ID3D11InputLayout  *pInputLayout;
	ID3D11Buffer       *pMatrixBuffer;
	ID3D11SamplerState *pSampleState;
	WCHAR *pVS_File;
	WCHAR *pPS_File;
};


#endif // ! SHADERCLASS_H
