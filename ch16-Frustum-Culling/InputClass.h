#ifndef INPUTCLASS_H
#define INPUTCLASS_H

#define DIRECTINPUT_VERSION 0X0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass &);
	~InputClass();

	bool Init(HINSTANCE , HWND, int, int);
	void Shutdown();
	bool Frame();
	
	bool IsEscapePressed();
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();

	void GetMouseLocation(int &, int &);

private:
    bool ReadKeyBoard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8 *pDirectInput;
	IDirectInputDevice8 *pKeyboard;
	IDirectInputDevice8 *pMouse;

	unsigned char keyboardState[256];
	DIMOUSESTATE mouseState;
	int screenWidth, screenHeight;
	int mouseX, mouseY;
};

#endif //InputClass