#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& rhs)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Init(int screenWidth, int screenHeight, HWND hwnd)
{
	return true;
}


void GraphicsClass::Shutdown()
{
	return;
}


bool GraphicsClass::Frame()
{
	return true;
}


bool GraphicsClass::Render()
{
	return true;
}