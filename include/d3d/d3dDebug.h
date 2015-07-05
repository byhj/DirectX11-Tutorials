#ifndef D3DDEBUG_H
#define D3DDEBUG_H

#include <dxerr.h>


#ifndef DebugHR()
#define DebugHR(hr)                                                  \
	{                                                                \
    HRESULT result = hr;                                             \
	if(FAILED(result))                                               \
	{                                                                \
		DXTrace(__FILE__, (DWORD)__LINE__, result, L"Error", true);  \
	}                                                                \
	}
#endif


#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

#define SafeDelete(x) { delete x; x = 0; }


#endif

