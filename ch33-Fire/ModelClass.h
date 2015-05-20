#ifndef MODELCLASS_H
#define MODELCLASS_H

#include <d3d11.h>
#include <D3DX10math.h>

#include "TextureClass.h"

#include <fstream>

class ModelClass 
{

public:
	ModelClass();
	ModelClass(const ModelClass &);
	~ModelClass();

	bool Init(ID3D11Device *, WCHAR *,WCHAR *,WCHAR *, char *);
	void Shutdown();
	void Render(ID3D11DeviceContext *);

	int GetIndexCount();
	//pass texture resource to shaders
	ID3D11ShaderResourceView* GetTexture1();
	ID3D11ShaderResourceView* GetTexture2();
	ID3D11ShaderResourceView* GetTexture3();


private:
	struct Vertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Tex;
		D3DXVECTOR3 Normal;
	};

	struct ModelVertex
	{
		float x, y , z;
		float u, v;
		float nx, ny, nz;
	};

private:
	bool InitBuffer(ID3D11Device *);
	void ShutdownBuffer();
	void RenderBuffer(ID3D11DeviceContext *);
	bool LoadTexture(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	void ShutdownTexture();

	bool LoadModel(char *);
	void ShoutdownModel();

private:
	ID3D11Buffer *pVertexBuffer;
	ID3D11Buffer *pIndexBuffer;
	TextureClass *m_Texture1, *m_Texture2, *m_Texture3;
	ModelVertex  *pModelVertex;

	int VertexCount, IndexCount;
};
#endif