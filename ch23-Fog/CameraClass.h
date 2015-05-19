#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include <D3DX10math.h>

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass &);
	~CameraClass();
   
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX &);

private:
	float PosX, PosY, PosZ;
	float RotX, RotY, RotZ;
	D3DXMATRIX ViewMatrix;
};
#endif