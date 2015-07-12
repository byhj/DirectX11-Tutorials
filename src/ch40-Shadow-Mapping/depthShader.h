#ifndef DEPTHSHADER_H
#define DEPTHSHADER_H

#include "d3d/d3dShader.h"

class DepthShader
{
public:
	DepthShader(){}
	~DepthShader(){}

	void Init(ID3D11Device *pD3D11Device, HWND hWnd);
	void Use(ID3D11DeviceContext *pD3D11DeviceContext);

private:
	Shader depthShader;
};
#endif
