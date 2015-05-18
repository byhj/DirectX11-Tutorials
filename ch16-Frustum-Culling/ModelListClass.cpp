#include "ModelListClass.h"

ModelListClass::ModelListClass()
{
	pModelInfoList = 0;
}


ModelListClass::ModelListClass(const ModelListClass& other)
{
}


ModelListClass::~ModelListClass()
{
}

bool ModelListClass::Init(int numModels)
{
	int i;
	float red, green, blue;

	// Store the number of models.
	modelCount = numModels;

	// Create a list array of the model information.
	pModelInfoList = new ModelInfoType[modelCount];
	if(!pModelInfoList)
	{
		return false;
	}
	// Seed the random generator with the current time.
	srand((unsigned int)time(NULL));

	// Go through all the models and randomly generate the model color and pos.
	for(i=0; i<modelCount; i++)
	{
		// Generate a random color for the model.
		red = (float)rand() / RAND_MAX;
		green = (float)rand() / RAND_MAX;
		blue = (float)rand() / RAND_MAX;

		pModelInfoList[i].color = D3DXVECTOR4(red, green, blue, 1.0f);

		// Generate a random pos in front of the viewer for the mode.
		pModelInfoList[i].posX = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
		pModelInfoList[i].posY = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
		pModelInfoList[i].posZ = ((((float)rand()-(float)rand())/RAND_MAX) * 10.0f) + 5.0f;
	}

	return true;
}

void ModelListClass::Shutdown()
{
	// Release the model information list.
	if(pModelInfoList)
	{
		delete [] pModelInfoList;
		pModelInfoList = 0;
	}

	return;
}

int ModelListClass::GetModelCount()
{
	return modelCount;
}

void ModelListClass::GetData(int index, float& posX, float& posY, float& posZ, D3DXVECTOR4& color)
{
	posX = pModelInfoList[index].posX;
	posY = pModelInfoList[index].posY;
	posZ = pModelInfoList[index].posZ;

	color = pModelInfoList[index].color;

	return;
}
