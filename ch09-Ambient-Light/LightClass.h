#ifndef LIGHTCLASS_H
#define LIGHTCLASS_H

#include <D3DX10math.h>

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass &);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();

private:
	D3DXVECTOR4 ambientColor;
	D3DXVECTOR4 diffuseColor;
	D3DXVECTOR3 direction;
};
#endif