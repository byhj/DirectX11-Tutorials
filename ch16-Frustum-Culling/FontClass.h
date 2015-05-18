#ifndef FONTCLASS_H
#define FONTCLASS_H

#include <d3d11.h>
#include <d3dx10math.h>

#include "TextureClass.h"

class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};
	struct Vertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 Tex;
	};
public:
	FontClass();
	FontClass(const FontClass &);
	~FontClass();

	bool Init(ID3D11Device *, char *, WCHAR *);
	void Shutdown();

	ID3D11ShaderResourceView *GetTexture();
	void BuildVAO(void *, char *, float, float);

private:
	bool LoadFontData(char *);
	void ShutdownFontData();
	bool LoadTexture(ID3D11Device*, WCHAR *);
	void ShutdownTexture();

private:
     FontType *pFont;
	 TextureClass *pTexture;
};


#endif