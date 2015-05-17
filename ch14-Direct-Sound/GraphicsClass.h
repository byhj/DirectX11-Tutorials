#ifndef GRAPHICSCLASS_H
#define GRAPHICSCLASS_H

#include <windows.h>

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ShaderClass.h"
#include "FontShader.h"
#include "LightClass.h"
#include "FontClass.h"

#include "TextClass.h"

//Global
const bool FULL_SCREEN   = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR  = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass &);
	~GraphicsClass();

	bool Init(int, int, HWND);
	void Shutdown();
	bool Render(int, int );

private:
	bool RenderGraphics(float );

	D3DClass     *pD3D;
	CameraClass  *pCamera;
	ModelClass   *pModel;
	ShaderClass  *pShader;
	FontShader   *pBitMapShader;
	LightClass   *pLight;

	TextClass* pText;
};
#endif //GraphicsClass