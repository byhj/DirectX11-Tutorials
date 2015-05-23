#ifndef GRAPHICSCLASS_H
#define GRAPHICSCLASS_H

#include <windows.h>

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ShadowShaderClass.h"
#include "DepthShaderClass.h"
#include "LightClass.h"
#include "rendertextureclass.h"

//Global
const bool FULL_SCREEN   = false;
const bool VSYNC_ENABLED = false;

const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass &);
	~GraphicsClass();

	bool Init(int, int, HWND);
	void Shutdown();
	bool Render();
	bool RenderToTexture();
	bool RenderScene();

private:
	bool RenderGraphics(float);

	D3DClass    *pD3D;
	CameraClass *pCamera;
    ModelClass *pCubeModel, *pGroundModel, *pSphereModel;
	ShadowShaderClass *pShadowShader;
	DepthShaderClass *pDepthShader;
	RenderTextureClass   *pRenderTexture;
	LightClass  *pLight;
};
#endif //GraphicsClass