#ifndef D3D_H
#define D3D_H

#include <string>
#include <windows.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <dxerr.h>

class D3DApp
{
		public:
			D3DApp(HINSTANCE hInstance);
			virtual ~D3DApp();
			HINSTANCE AppInst() const;
			HWND      MainWnd() const;
			float     AspectRatio() const;

			int Run();

			virtual bool Init();
			virtual void Resize(); 
			virtual void UpdateScene(float dt) = 0;
			virtual void RenderScene() = 0; 
			virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

			// Convenience overrides for handling mouse input.
			virtual void OnMouseDown(WPARAM btnState, int x, int y){ }
			virtual void OnMouseUp(WPARAM btnState, int x, int y)  { }
			virtual void OnMouseMove(WPARAM btnState, int x, int y){ }

		protected:
			bool InitWindow();
			bool InitD3D();

		protected:

			HINSTANCE hInstance;
			HWND      hWnd;
			bool      Paused;
			bool      Minimized;
			bool      Maximized;
			bool      Resizing;
			UINT      m4xMsaaQuality;

			//object handle
			ID3D11Device              *pDevice;
			ID3D11DeviceContext       *pDeviceContext;
			IDXGISwapChain            *pSwapChain;
			ID3D11Texture2D           *pDepthStencilBuffer;
			ID3D11RenderTargetView    *pRenderTargetView;
			ID3D11DepthStencilView    *pDepthStencilView;
			D3D11_VIEWPORT             Viewport;

			// Derived class should set these in derived constructor to customize starting values.
			std::wstring WindowTitle;
			D3D_DRIVER_TYPE  DriverType;
			int ClientWidth;
			int ClientHeight;
			bool Enable4xMSAA;
};

#endif // D3DAPP_H