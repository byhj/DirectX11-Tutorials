/* The frame work will begin with four items. 
 * It will have a WinMain function to handle the entry point of the application. 
 * It will also have a system class that encapsulates the entire application that will be called from within the WinMain function. 
 * Inside the system class we will have a input class for handling user input and a graphics class for handling the DirectX graphics code. 
 * Here is a diagram of the framework setup:
 *
 *                 WinMain
 *                    |
 *               SystemClass
 *                    |
 *         InputClass   GraphicsClass
 */   


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