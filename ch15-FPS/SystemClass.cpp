#include "SystemClass.h"

SystemClass::SystemClass()
	:pInput(0),
	 pGraphics(0),
	 pFps(0),
	 pCpu(0),
	 pTimer(0)
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
	result = pInput->Init(hInstance, hWnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

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
	// Create the fps object.
	pFps = new FpsClass;
	if(!pFps)
	{
		return false;
	}

	// Initialize the fps object.
	pFps->Init();
	///////////////////////////////////////////////////////////////////////
	// Create the cpu object.
	pCpu = new CpuClass;
	if(!pCpu)
	{
		return false;
	}

	// Initialize the cpu object.
	pCpu->Init();

	/////////////////////////////////////////////////////////
	// Create the timer object.
	pTimer = new TimerClass;
	if(!pTimer)
	{
		return false;
	}

	// Initialize the timer object.
	result = pTimer->Init();
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}
	return true;
}

void SystemClass::Shutdown()
{
	// Release the timer object.
	if(pTimer)
	{
		delete pTimer;
		pTimer = 0;
	}

	// Release the cpu object.
	if(pCpu)
	{
		pCpu->Shutdown();
		delete pCpu;
		pCpu = 0;
	}

	// Release the fps object.
	if(pFps)
	{
		delete pFps;
		pFps = 0;
	}
	if (pGraphics)
	{
		pGraphics->Shutdown();
		delete pGraphics;
		pGraphics = 0;
	}

	if (pInput)
	{
		pInput->Shutdown();
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
		if (pInput->IsEscapePressed())
		{
			done = true;
		}

	};
}

bool SystemClass::Render()
{
	bool result;
	int mouseX, mouseY;

	// Update the system stats.
	pTimer->Frame();
	pFps->Frame();
	pCpu->Frame();

	//Check if the user pressed esacpe 
    result = pInput->Frame();
	if (!result)
	{
		return false;
	}
	pInput->GetMouseLocation(mouseX, mouseY);

	result = pGraphics->Render(mouseX, mouseY, pFps->GetFps(), pCpu->GetCpuPercentage(), pTimer->GetTime() );
	if(!result)
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
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
