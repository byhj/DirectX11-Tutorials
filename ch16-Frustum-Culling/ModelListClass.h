#ifndef MODELLISTCLASS_H
#define MODELLISTCLASS_H

#include <D3DX10math.h>
#include <stdlib.h>
#include <time.h>

class ModelListClass
{
private:
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
		float posX, posY, posZ;
	};

public:
	ModelListClass();
	ModelListClass(const ModelListClass &);
	~ModelListClass();

	 bool Init(int);
	 void Shutdown();
	 int GetModelCount();
	 void GetData(int, float &, float &, float &, D3DXVECTOR4 &);

private:
	int modelCount;
	ModelInfoType *pModelInfoList;
};

#endif