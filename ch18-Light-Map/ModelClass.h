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

	bool Init(ID3D11Device *, char *, WCHAR *, WCHAR *);
	void Shutdown();
	void Render(ID3D11DeviceContext *);

	int GetIndexCount();
	//pass texture resource to shaders
	ID3D11ShaderResourceView **GetTextureArray();

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
	bool LoadTextures(ID3D11Device*, WCHAR *, WCHAR *);
	void ShutdownTexture();

	bool LoadModel(char *);
	void ShoutdownModel();

private:
	ID3D11Buffer *pVertexBuffer;
	ID3D11Buffer *pIndexBuffer;
	TextureClass *pTexture;	
	ModelVertex  *pModelVertex;

	int VertexCount, IndexCount;
};

#endif

