#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass *pSystem;
	bool result;

	//Create the system object
	pSystem = new SystemClass;
	if (!pSystem)
	{
	   return 0;
	}
	result = pSystem->Init();
    if (result)
	{
		pSystem->Run();
	}

	//we Shutdown after system is done
	pSystem->Shutdown();
	delete pSystem;
	pSystem = 0;

	return 0;
}