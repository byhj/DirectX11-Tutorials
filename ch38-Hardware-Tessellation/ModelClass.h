#ifndef MODELCLASS_H
#define MODELCLASS_H

#include <d3d11.h>
#include <D3DX10math.h>

class ModelClass 
{

public:
	ModelClass();
	ModelClass(const ModelClass &);
	~ModelClass();

	bool Init(ID3D11Device *);
	void Shutdown();
	void Render(ID3D11DeviceContext *);

	int GetIndexCount();

private:
	struct Vertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR4 Color;
	};
private:
	bool InitBuffer(ID3D11Device *);
	void ShutdownBuffer();
	void RenderBuffer(ID3D11DeviceContext *);

	ID3D11Buffer *pVertexBuffer;
	ID3D11Buffer *pIndexBuffer;
	int VertexCount, IndexCount;
};
#endif