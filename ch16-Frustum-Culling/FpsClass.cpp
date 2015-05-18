#include "FpsClass.h"

FpsClass::FpsClass()
{

}

FpsClass::FpsClass(const FpsClass &)
{

}

FpsClass::~FpsClass()
{

}

void FpsClass::Init()
{
	fps = 0;
	count = 0;
	startTime = timeGetTime();

	return ;
}

void FpsClass::Frame()
{
	count++;

    //More than one second
	if (timeGetTime() >= (startTime + 1000))
	{
		fps = count;
		count = 0;
		startTime = timeGetTime();
	}
}

int FpsClass::GetFps()
{
	return fps;
}