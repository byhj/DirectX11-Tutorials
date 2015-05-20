#ifndef BITMAPCLASS_H
#define BITMAPCLASS_H

#include <d3d11.h>
#include <d3dx10math.h>

#include "TextureClass.h"

class BitmapClass
{
private:
	struct Vertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 Tex;
	};
public:
	BitmapClass();
	BitmapClass(const BitmapClass &);
	~BitmapClass();

	bool Init(ID3D11Device *, int , int, WCHAR *, int, int );
	void Shutdown();
	bool Render(ID3D11DeviceContext *, int, int);
	
	int GetIndexCount();
	ID3D11ShaderResourceView *GetTexture();

private:
	bool InitBuffer(ID3D11Device *);
	void ShutdownBuffer();
	bool UpdateBuffer(ID3D11DeviceContext*, int, int);
	void RenderBuffer(ID3D11DeviceContext *);

	bool LoadTexture(ID3D11Device*, WCHAR *);
	void ShutdownTexture();

private:
	ID3D11Buffer *pVertexBuffer, *pIndexBuffer;
	int VertexCount, IndexCount;
	TextureClass *pTexture;

	int screenWidth, screenHeight;
	int bitmapWidth, bitmapHeight;
	int lastPosX, lastPosY;
};


#endif