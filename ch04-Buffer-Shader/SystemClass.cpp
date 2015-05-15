#include "SystemClass.h"

SystemClass::SystemClass():pInput(0), pGraphics(0)
{
}

SystemClass::SystemClass(const SystemClass & rhs)
{
}

SystemClass::~SystemClass()
{
}

//Init (Window-->Input-->Graphics)

bool SystemClass::Init()
{
	int screenWidth = 0, screenHeight = 0;
	bool result;
	InitWindow(screenWidth, screenHeight);

	//Create Input Class Object
	pInput = new InputClass;
	if (!pInput)
	{
		return false;
	}
	pInput->Init();

	//Create Graphics Class Object
	pGraphics = new GraphicsClass;
	if(!pGraphics)
	{
		return false;
	}
	result = pGraphics->Init(screenWidth, screenHeight, hWnd);
	if(!result)
	{
		return false;
	}

	return true;
}


void SystemClass::Shutdown()
{
	//Release Graphics Object
	if (pGraphics)
	{
		pGraphics->Shutdown();
		delete pGraphics;
		pGraphics = 0;
	}

	//Release Input Object
	if (pInput)
	{
		delete pInput;
		pInput = 0;
	}

    ShutdownWindows();
}


void SystemClass::Run()
{
	MSG msg;
	bool result;
	ZeroMemory(&msg, sizeof(MSG));
	bool done = false;

	while (!done)
	{
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//Do The Frame Process
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}
	return ;
}

bool SystemClass::Frame()
{
	bool result;

	if (pInput->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	result = pGraphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	 case WM_KEYDOWN:
		{
			pInput->KeyDown( (unsigned int)wParam );
			return 0;
		}
	 case WM_KEYUP:
		 {
			 pInput->KeyUp((unsigned int)wParam);
			 return 0;
		 }

		 // Any other messages send to the default message handler as our application won't make use of them.
	 default:
		 {
			 return DefWindowProc(hWnd, uMsg, wParam, lParam);
		 }
	} 

}

void SystemClass::InitWindow(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to this object.
	AppHandle = this;

	// Get the instance of this application.
	hInstance = GetModuleHandle(NULL);

	// Give the application a name.
	AppName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = AppName;
	wc.cbSize        = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 1000;
		screenHeight = 800;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	hWnd = CreateWindowEx(	
		NULL,	           
		AppName, AppName,
		WS_OVERLAPPEDWINDOW,	
		posX, posY, 
		screenWidth, screenHeight,
		NULL,
		NULL,
		hInstance,	
		NULL
		);


	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	return;
}



void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(hWnd);
	hWnd = NULL;

	// Remove the application instance.
	UnregisterClass(AppName, hInstance);
	hInstance = NULL;

	// Release the pointer to this class.
	AppHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
	case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
	default:
		{
			return AppHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
