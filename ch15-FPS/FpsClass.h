#ifndef FPSCLASS_H
#define FPSCLASS_H

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>

class FpsClass
{
public:
	FpsClass();
	FpsClass(const FpsClass &);
	~FpsClass();

	void Init();
	void Frame();
	int GetFps();

private:
	int fps, count;
	unsigned long startTime;
};

#endif
