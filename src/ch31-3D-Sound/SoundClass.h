#ifndef SOUNDCLASS_H
#define SOUNDCLASS_H

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class SoundClass
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	SoundClass();
	SoundClass(const SoundClass &);
	~SoundClass();

	bool Init(HWND, char *soundFile);
	void Shutdown();

private:
	bool InitDirectSound(HWND);
	void ShutdownDirectSound();
	bool LoadWaveFile(char *, IDirectSoundBuffer8**, IDirectSound3DBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**, IDirectSound3DBuffer8**);

	bool PlayWaveFile();

private:
	IDirectSound8 *pDirectSound;
	IDirectSoundBuffer  *pPrimaryBuffer;
	IDirectSoundBuffer8  *pSecondaryBuffer;
	IDirectSound3DListener8* m_listener;
	IDirectSound3DBuffer8* m_secondary3DBuffer1;
};

#endif