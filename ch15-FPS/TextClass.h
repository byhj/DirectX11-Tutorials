#ifndef TextCLASS_H
#define TextCLASS_H

#include <d3d11.h>
#include <d3dx10math.h>

#include "TextureClass.h"
#include "FontClass.h"
#include "FontShader.h"

class TextClass
{
private:
	struct Vertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 Tex;
	};
	struct Sentence
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

public:
	TextClass();
	TextClass(const TextClass &);
	~TextClass();

	bool Init(ID3D11Device *, ID3D11DeviceContext *, HWND, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext *, D3DXMATRIX, D3DXMATRIX);
	bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView *GetTexture();

private:
	bool InitBuffer(Sentence**, int, ID3D11Device*);
	void ShutdownBuffer(Sentence **);
	bool UpdateBuffer(Sentence *, char *, int, int, float, float, float, ID3D11DeviceContext*);
	bool RenderBuffer(ID3D11DeviceContext *, Sentence *, D3DXMATRIX, D3DXMATRIX);

private:
    FontClass *pFont;
    FontShader *pFontShader;
	int screenWidth, screenHeight;
	D3DXMATRIX BaseView;

	Sentence *pSentence1;
	Sentence *pSentence2;
};


#endif