#include "TextureClass.h"

TextureClass::TextureClass()
	:pTextureResourceView(0)
{

}

TextureClass::TextureClass(const TextureClass &rhs)
{

}

TextureClass::~TextureClass()
{

}

bool TextureClass::Init(ID3D11Device *pD3D11Device, WCHAR *textureFile)
{
	HRESULT result;

	//Load the texture file
	result = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, textureFile, NULL, NULL, &pTextureResourceView, NULL);
	if ( FAILED(result) )
	{
		return false;
	}
	return true;
}

void TextureClass::Shutdown()
{
	if (!pTextureResourceView)
	{
		pTextureResourceView->Release();
		pTextureResourceView = 0;
	}

	return;
}

ID3D11ShaderResourceView *TextureClass::GetTexture()
{
	return pTextureResourceView;
}