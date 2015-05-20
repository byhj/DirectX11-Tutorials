#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
	:pD3D(0),
	 pCamera(0),
	 pModel(0),
	 pShader(0),
	 pLight(0)
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& rhs)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Init(int screenWidth, int screenHeight, HWND hWnd)
{
	bool result;

	/////////////////////// D3DClass /////////////////////////////////////

	pD3D = new D3DClass;
	if(!pD3D)
	{
		return false;
	}

	result = pD3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	////////////////////////Camera  Class///////////////////////////

	pCamera = new CameraClass;
	if (!pCamera)
	{
		return false;
	}
	pCamera->SetPosition(0.0f, 0.0f, -5.0f);

	//////////////////////// Model Class ////////////////////////////
	pModel = new ModelClass;
	if (!pModel)
	{
		return false;
	}

	result = pModel->Init(pD3D->GetDevice(), "cube.txt", L"../common/media/texture/stone02.dds", 
		L"../common/media/texture/bump02.dds", L"../common/media/texture/spec02.dds");
	
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	///////////////////////Shader Class ////////////////////////////
	pShader = new ShaderClass;
	if (!pShader)
	{
       return false;
	}

	result = pShader->Init(pD3D->GetDevice(), hWnd);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	///////////////////////////////////////////////////////////////////

	pLight = new LightClass;
	if (!pLight)
	{
		return false;
	}

	pLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	pLight->SetDirection(0.0f, 0.0f, 1.0f);
	pLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	pLight->SetSpecularPower(16.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	if (pLight)
	{
		delete pLight;
		pLight = 0;
	}

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

	//Release d3d object
	if(pD3D)
	{
		pD3D->Shutdown();
		delete pD3D;
		pD3D = 0;
	}
	return;
}


bool GraphicsClass::Render()
{
	bool result;

	static float rot = 0.0f;
	rot += (float)D3DX_PI * 0.00001f;
	if (rot > 360.0f)
	{
		rot = 0.0f;
	}
	result = RenderGraphics(rot);

	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::RenderGraphics(float rot)
{
	D3DXMATRIX View, Proj, World;
	bool result;

	static float textureTranslation = 0.0f;
	textureTranslation += 0.0001f;
	if(textureTranslation > 1.0f)
	{
		textureTranslation -= 1.0f;
	}

	pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
    pCamera->Render();

	pCamera->GetViewMatrix(View);
	pD3D->GetWorld(World);
	pD3D->GetProj(Proj);
	pModel->Render( pD3D->GetDeviceContext() );

	//D3DXMatrixRotationX(&World, rot);
	D3DXMatrixRotationY(&World, rot);
	//D3DXMatrixRotationZ(&World, rot);

	//Render the Model Using Shader
	result = pShader->Render(pD3D->GetDeviceContext(), pModel->GetIndexCount(), World, View, Proj, 
		                     pModel->GetTextureArray(), pLight->GetDirection(), pLight->GetDiffuseColor(),
							 pCamera->GetPosition(), pLight->GetSpecularColor(), pLight->GetSpecularPower(),textureTranslation);
    if (!result)
	{
		return false;
	}

	pD3D->EndScene();
	return true;
}