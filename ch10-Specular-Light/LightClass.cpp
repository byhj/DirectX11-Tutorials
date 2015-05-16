#include "LightClass.h"

LightClass::LightClass()
{

}

LightClass::LightClass(const LightClass &rhs)
{

}

LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float r, float g, float b, float a)
{
	ambientColor = D3DXVECTOR4(r, g, b, a);

	return;
}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	diffuseColor = D3DXVECTOR4(r, g, b, a);

	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	direction = D3DXVECTOR3(x, y, z);
	return ;
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return ambientColor;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return direction;
}

D3DXVECTOR4	LightClass::GetDiffuseColor()
{
	return diffuseColor;
}

