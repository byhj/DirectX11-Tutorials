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
void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
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

void LightClass::SetSpecularPower(float p)
{
	specularPower = p;

	return;
}

void LightClass::SetLookAt(float x, float y, float z)
{
	m_lookAt.x = x;
	m_lookAt.y = y;
	m_lookAt.z = z;
	return;
}

void LightClass::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
	return;
}
D3DXVECTOR3 LightClass::GetPosition()
{
	return m_position;
}
void LightClass::GenerateViewMatrix()
{
	D3DXVECTOR3 up;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_lookAt, &up);

	return;
}

void LightClass::GenerateProjectionMatrix(float screenDepth, float screenNear)
{
	float fieldOfView, screenAspect;


	// Setup field of view and screen aspect for a square light source.
	fieldOfView = (float)D3DX_PI / 2.0f;
	screenAspect = 1.0f;

	// Create the projection matrix for the light.
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	return;
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

void LightClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}


void LightClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return direction;
}

D3DXVECTOR4	LightClass::GetDiffuseColor()
{
	return diffuseColor;
}

D3DXVECTOR4	LightClass::GetSpecularColor()
{
	return specularColor;
}

float LightClass::GetSpecularPower()
{
	return specularPower;
}