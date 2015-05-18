#include "TextureClass.h"

TextureClass::TextureClass()
{
	pTextures[0] = 0;
	pTextures[1] = 0;

}

TextureClass::TextureClass(const TextureClass &rhs)
{

}

TextureClass::~TextureClass()
{

}

bool TextureClass::Init(ID3D11Device *pD3D11Device, WCHAR *textureFile1, WCHAR *textureFile2)
{
	HRESULT result;

	//Load the texture file
	result = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, textureFile1, NULL, NULL, &pTextures[0], NULL);
	if ( FAILED(result) )
	{
		return false;
	}

	result = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, textureFile2, NULL, NULL, &pTextures[1], NULL);
	if ( FAILED(result) )
	{
		return false;
	}
	return true;
}

void TextureClass::Shutdown()
{

	for (int i = 0; i != 2; ++i)
	{
		pTextures[i]->Release();
		pTextures[i] = 0;
	}

	return;
}

ID3D11ShaderResourceView ** TextureClass::GetTextureArray()
{
	return pTextures;
}