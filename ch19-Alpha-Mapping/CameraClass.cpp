#include "CameraClass.h"

CameraClass::CameraClass()
	:PosX(0),
	 PosY(0),
	 PosZ(0),
	 RotX(0),
	 RotY(0),
	 RotZ(0)
{

}

CameraClass::CameraClass(const CameraClass &Other)
{

}

CameraClass::~CameraClass()
{

}

void CameraClass::SetPosition(float x, float y, float z)
{
	PosX = x;
	PosY = y;
	PosZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	RotX = x;
	RotY = y;
	RotZ = z;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(PosX, PosY, PosZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(RotX, RotY, RotZ);
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = ViewMatrix;
	return;
}


void CameraClass::Render()
{
	D3DXVECTOR3 up, pos, target;
    float  yaw, pitch, roll;
	D3DXMATRIX RotationMatrix;
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	pos.x = PosX;
	pos.y = PosY;
	pos.z = PosZ;

	target.x = 0.0f;
	target.y = 0.0f;
	target.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = RotX * 0.0174532925f;
	yaw   = RotY * 0.0174532925f;
	roll  = RotZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&target, &target, &RotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &RotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	target = pos + target;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&ViewMatrix, &pos, &target, &up);
}