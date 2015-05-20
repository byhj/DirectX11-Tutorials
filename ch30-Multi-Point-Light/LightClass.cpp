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

void LightClass::SetPosition(float x, float y, float z)
{
	position = D3DXVECTOR4(x, y, z, 1.0f);

	return;
}

D3DXVECTOR4 LightClass::GetPosition()
{
	return position;
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

void LightClass::SetSpecularColor(float r, float g, float b, float a)
{
	specularColor = D3DXVECTOR4(r, g, b, a);

	return;
}

void LightClass::SetSpecularPower(float power)
{
	specularPower = power;
	return;
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

D3DXVECTOR4 LightClass::GetSpecularColor()
{
	return specularColor;
}

float LightClass::GetSpecularPower()
{
	return specularPower;
}
