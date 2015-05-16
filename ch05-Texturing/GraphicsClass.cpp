#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
	:pD3D(0),
	 pCamera(0),
	 pModel(0),
	 pShader(0)
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
	bool result;

	////////    D3DClass   //////////////////////////////////////
	pD3D = new D3DClass;
	if(!pD3D)
	{
		return false;
	}

	result = pD3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	////////  Camera  Class///////////////////////////////////
	pCamera = new CameraClass;
	if (!pCamera)
	{
		return false;
	}
	pCamera->SetPosition(0.0f, 0.0f, -10.0f);

	///////////  Model Class //////////////////////////////////
	pModel = new ModelClass;
	if (!pModel)
	{
		return false;
	}
	result = pModel->Init( pD3D->GetDevice() );
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	////////// Shader Class /////////////////////////////////////////
	pShader = new ShaderClass;
	if (!pShader)
	{
       return false;
	}
	result = pShader->Init(pD3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	///////////////////////////////////////////////////////////////////
	return true;
}


void GraphicsClass::Shutdown()
{

	// Release the color shader object.
	if(pShader)
	{
		pShader->Shutdown();
		delete pShader;
		pShader = 0;
	}

	// Release the model object.
	if(pModel)
	{
		pModel->Shutdown();
		delete pModel;
		pModel = 0;
	}

	// Release the camera object.
	if(pCamera)
	{
		delete pCamera;
		pCamera = 0;
	}

	if(pD3D)
	{
		pD3D->Shutdown();
		delete pD3D;
		pD3D = 0;
	}
	return;
}


bool GraphicsClass::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	D3DXMATRIX View, Proj, Model;
	bool result;

	pD3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
    pCamera->Render();
	pCamera->GetViewMatrix(View);
	pD3D->GetWorld(Model);
	pD3D->GetProj(Proj);
	pModel->Render( pD3D->GetDeviceContext() );

	//Render the Model Using Shader
	result = pShader->Render(pD3D->GetDeviceContext(), pModel->GetIndexCount(), Model, View, Proj);
    if (!result)
	{
		return false;
	}

	pD3D->EndScene();
	return true;
}