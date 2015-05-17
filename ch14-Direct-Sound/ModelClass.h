#ifndef MODELCLASS_H
#define MODELCLASS_H

#include <d3d11.h>
#include <D3DX10math.h>
#include <vector>

#include "TextureClass.h"

#include <fstream>

class ModelClass 
{

public:
	ModelClass();
	ModelClass(const ModelClass &);
	~ModelClass();

	bool Init(ID3D11Device *, WCHAR *, char *);
	void Shutdown();
	void Render(ID3D11DeviceContext *);

	int GetIndexCount();
	//pass texture resource to shaders
	ID3D11ShaderResourceView *GetTexture();

private:
	struct Vertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 Tex;
		D3DXVECTOR3 Normal;
	};

private:
	bool InitBuffer(ID3D11Device *);
	void ShutdownBuffer();
	void RenderBuffer(ID3D11DeviceContext *);
	bool LoadTexture(ID3D11Device*, WCHAR *);
	void ShutdownTexture();

	bool LoadObjModel(char *);
	void ShoutdownModel();

private:
	ID3D11Buffer *pVertexBuffer;
	ID3D11Buffer *pIndexBuffer;
	TextureClass *pTexture;	
	std::vector<Vertex> VertexData;
	std::vector<unsigned int> IndexData;

	int VertexCount, IndexCount;
};
#endif