#include "TextureMgr.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{

	void TextureMgr::LoadTexture(ID3D11Device *pD3D11Device, std::wstring texFile)
	{
      HRESULT hr;
	  ComPtr<ID3D11ShaderResourceView> pTexture;
	  hr = CreateDDSTextureFromFile(pD3D11Device, m_TextureDir + texFile, NULL, &pTexture);
	  m_Textures.push_back(pTexture);
	}

	ComPtr<ID3D11ShaderResourceView> TextureMgr::GetTexture(std::wstring texName)
	{
		auto index = m_TextureMapIndex[texName];
		return m_Textures[index];
	}
}