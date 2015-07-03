#ifndef D3DFONT_H
#define D3DFONT_H

#include "d3dDebug.h"

#include <FW1FontWrapper.h>
#pragma  comment(lib, "FW1FontWrapper.lib")

namespace byhj
{
	class D3DFont;
}

class D3DFont
{
public:
	D3DFont(): m_pFW1Factory(NULL), m_pFontWrapper(NULL) {}

    void init(ID3D11Device *pD3D11Device)
	{
		HRESULT hr = FW1CreateFactory(FW1_VERSION, &m_pFW1Factory);
		hr = m_pFW1Factory->CreateFontWrapper(pD3D11Device, L"Arial", &m_pFontWrapper);
		DebugHR(hr);
		m_pFW1Factory->Release();
	}
	void drawText(ID3D11DeviceContext *pD3D11DeivceContext, WCHAR *text, 
		          float fontSize, float posX, float posY);

	void drawFps(ID3D11DeviceContext *pD3D11DeviceContext, UINT fps);

private:
	IFW1Factory     *m_pFW1Factory;
	IFW1FontWrapper *m_pFontWrapper;
};

void D3DFont::drawFps(ID3D11DeviceContext *pD3D11DeviceContext, UINT fps)
{
	static WCHAR frameStr[255];
	wsprintfW(frameStr, L"FPS: %u", fps);

	m_pFontWrapper->DrawString(
		pD3D11DeviceContext,
		frameStr,// String
		22.0f,// Font size
		10.0f,// X position
		10.0f,// Y position
		0xff0099ff,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
		);
}

void D3DFont::drawText(ID3D11DeviceContext *pD3D11DeivceContext, WCHAR *text, 
					  float fontSize, float posX, float posY)
{
	m_pFontWrapper->DrawString(
		pD3D11DeivceContext,
		text,// String
		fontSize,// Font size
		posX,// X position
		posY,// Y position
		0xff0099ff,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
		);
}

#endif
