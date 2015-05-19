#include "SystemClass.h"

SystemClass::SystemClass()
	:pInput(0),
	 pGraphics(0)
{
}

SystemClass::SystemClass(const SystemClass & rhs)
{
}

SystemClass::~SystemClass()
{
}

//Init (Init Window-->Input-->Graphics object)

bool SystemClass::Init()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
    screenHeight = 0;

	///////////////////////////Init Window///////////////////////////
	InitWindow(screenWidth, screenHeight);

	///////////////////////////Init Input////////////////////////////
	pInput = new InputClass;
	if (!pInput)
	{
       return false;
	}
	pInput->Init();

	/////////////////////////Init Graphics////////////////////////////
	pGraphics = new GraphicsClass;
	if (!pGraphics)
	{
		return false;
	}
	result = pGraphics->Init(screenWidth, screenHeight, hWnd);
	if (!result)
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////
	return true;
}

void SystemClass::Shutdown()
{
	if (pGraphics)
	{
		pGraphics->Shutdown();
		delete pGraphics;
		pGraphics = 0;
	}

	if (pInput)
	{
		delete pInput;
		pInput = 0;
	}

	ShutdownWindow();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	ZeroMemory( &msg, sizeof(MSG) );
	bool done, result;
	done = false;

	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Render();
			if(!result)
			{
				done = true;
			}
		}

	};
}

bool SystemClass::Render()
{
	bool result;

	//Check if the user pressed esacpe 
	if (pInput->IsKeyDown(VK_ESCAPE) )
	{
		return false;
	}

	result = pGraphics->Render();
	if (!result)
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			pInput->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
	case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			pInput->KeyUp((unsigned int)wparam);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
	default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void SystemClass::InitWindow(int &screenWidth, int &screenHeight)
{

	hInstance = GetModuleHandle(NULL);
	AppName = L"DirectX11.0-Toturial";
 
	//Notice !!!!!, Call this object message handler
	AppHandle = this;

	WNDCLASSEX wc;	
	wc.cbSize = sizeof(WNDCLASSEX);	
	wc.style = CS_HREDRAW | CS_VREDRAW;	
	wc.lpfnWndProc = WndProc;	
	wc.cbClsExtra = NULL;	
	wc.cbWndExtra = NULL;	
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);	
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;	
	wc.lpszClassName = AppName;	
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); 

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Registering Class Failded",	L"Error", MB_OK | MB_ICONERROR);
		return ;
	}

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	DEVMODE dmScreenSettings;
	int posX, posY;

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
		// If windowed then set it to 1000x800 resolution.
		screenWidth  = 1000;
		screenHeight = 800;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	hWnd = CreateWindowEx(	
		NULL,	           
		AppName,	  
		AppName, 
		WS_OVERLAPPEDWINDOW,	
        posX, posY,
		screenWidth, screenHeight,
		NULL,
		NULL,
		hInstance,	
		NULL
		);

	if (!hWnd )	
	{
		MessageBox(NULL, L"Creating Window Failed", L"Error", MB_OK | MB_ICONERROR);
		return ;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);	

}


void SystemClass::ShutdownWindow()
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
