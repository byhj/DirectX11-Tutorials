#include "InputClass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass &rhs)
{
}

InputClass::~InputClass()
{
}

void InputClass::Init()
{
	for (int i = 0; i != 256; ++i)
		keys[i] = false;
}

void InputClass::KeyDown(unsigned int input)
{
	keys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
	keys[input] = false;
}

//Return what state the keys is in

bool InputClass::IsKeyDown(unsigned int key)
{
	return keys[key];
}


