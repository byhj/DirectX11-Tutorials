#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

#include <d3d11.h>
#include <D3DX11tex.h>

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass &);
	~TextureClass();
    
	bool Init(ID3D11Device *, WCHAR*, WCHAR*, WCHAR*);

	void Shutdown();
	ID3D11ShaderResourceView **GetTextureArray();
	ID3D11ShaderResourceView *GetTexture();

private:
	ID3D11ShaderResourceView *pTextures[3];

};
#endif