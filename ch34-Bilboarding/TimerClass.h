#ifndef TIMERCLASS_H
#define TIMERCLASS_H

#include <windows.h>


class TimerClass
{
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Init();
	void Frame();
	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

#endif