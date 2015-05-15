#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{

	bool result;

	// Create the system object.

	SystemClass* System;
	System = new SystemClass;

	if(!System)
	{
		return 0;
	}

	//Init and Run System

	result = System->Init();

	if(result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}