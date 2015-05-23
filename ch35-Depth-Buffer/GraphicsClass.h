#ifndef GRAPHICSCLASS_H
#define GRAPHICSCLASS_H

#include <windows.h>

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ShaderClass.h"
#include "LightClass.h"

//Global
const bool FULL_SCREEN   = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR  = 1.0f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass &);
	~GraphicsClass();

	bool Init(int, int, HWND);
	void Shutdown();
	bool Render();

private:
	bool RenderGraphics(float);

	D3DClass    *pD3D;
	CameraClass *pCamera;
	ModelClass  *pModel;
	ShaderClass *pShader;
	LightClass  *pLight;
};
#endif //GraphicsClass