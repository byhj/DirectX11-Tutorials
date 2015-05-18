#include "InputClass.h"

InputClass::InputClass()
	:pDirectInput(0),
	 pKeyboard(0),
	 pMouse(0)
{
}

InputClass::InputClass(const InputClass &rhs)
{
}

InputClass::~InputClass()
{
}

bool InputClass::Init(HINSTANCE hInstance, HWND hWnd, int ScreenWidth, int ScreenHeight)
{
	HRESULT result;

	//Store the screen size which will be used for positioning the mouse cursor
	screenWidth = ScreenWidth;
	screenHeight = ScreenHeight;

	mouseX = 0;
	mouseY = 0;
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDirectInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = pDirectInput->CreateDevice(GUID_SysKeyboard, &pKeyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	result = pKeyboard->SetCooperativeLevel(hWnd,  DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = pKeyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	result = pDirectInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = pMouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	if (pMouse)
	{
		pMouse->Unacquire();
		pMouse->Release();
		pMouse = 0;
	}

	if (pKeyboard)
	{
		pKeyboard->Unacquire();
		pKeyboard->Release();
		pKeyboard = 0;
	}

	if (pDirectInput)
	{
		pDirectInput->Release();
		pDirectInput = 0;
	}

	return ;
}


bool InputClass::Frame()
{
	bool result;
	result = ReadKeyBoard();
	if (!result)
	{
		return false;
	}

	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	ProcessInput();

	return true;
}

bool InputClass::ReadKeyBoard()
{
	HRESULT result;
	result = pKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
    if (FAILED(result))
	{
		if ( (result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED) )
		{
			pKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;
	result = pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(result))
	{
		if ( (result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED) )
		{
           pMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputClass::ProcessInput()
{

	//Update the location of the mouse cursor based on the change of the mouse location during the frame
	mouseX += mouseState.lX;
	mouseY += mouseState.lY;

	if (mouseX < 0)
	{
		mouseX = 0;
	}
	if (mouseY < 0)
	{
		mouseY = 0;
	}
	if (mouseX > screenWidth)
	{
		mouseX = screenWidth;
	}
	if (mouseY > screenHeight)
	{
		mouseY = screenHeight;
	}

	return;
}



bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputClass::IsLeftArrowPressed()
{
	if(keyboardState[DIK_LEFT] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsRightArrowPressed()
{
	if(keyboardState[DIK_RIGHT] & 0x80)
	{
		return true;
	}

	return false;
}

void InputClass::GetMouseLocation(int &mX, int &mY)
{
	mX = mouseX;
	mY = mouseY;
	return;
}