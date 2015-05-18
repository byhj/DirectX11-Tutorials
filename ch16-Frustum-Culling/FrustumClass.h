#ifndef FRUSTUMCLASS_H
#define FRUSTUMCLASS_H

#include <D3DX10math.h>

class FrustumClass
{
public:
	FrustumClass();
	FrustumClass(const FrustumClass&);
	~FrustumClass();

	void ContructFrustum(float, D3DXMATRIX, D3DXMATRIX);
	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);

private:
	//Cull face
	D3DXPLANE planes[6];

};
#endif