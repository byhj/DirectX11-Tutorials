#ifndef BITMAPCLASS_H
#define BITMAPCLASS_H

#include <d3d11.h>
#include <d3dx10math.h>

class DebugWindowClass
{
private:
	struct Vertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 Tex;
	};
public:
	DebugWindowClass();
	DebugWindowClass(const DebugWindowClass &);
	~DebugWindowClass();

	bool Init(ID3D11Device *, int , int,  int, int );
	void Shutdown();
	bool Render(ID3D11DeviceContext *, int, int);
	
	int GetIndexCount();

private:
	bool InitBuffer(ID3D11Device *);
	void ShutdownBuffer();
	bool UpdateBuffer(ID3D11DeviceContext*, int, int);
	void RenderBuffer(ID3D11DeviceContext *);


private:
	ID3D11Buffer *pVertexBuffer, *pIndexBuffer;
	int VertexCount, IndexCount;

	int screenWidth, screenHeight;
	int bitmapWidth, bitmapHeight;
	int lastPosX, lastPosY;
};


#endif