#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
	:pD3D(0),
	 pCamera(0),
	 pModel(0),
	 pShader(0),
	 pLight(0),
	 pBitmap(0),
	 pBitMapShader(0)
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
	pCamera->SetPosition(0.0f, 0.0f, -3.0f);

	//////////////////////// Model Class ////////////////////////////
	pModel = new ModelClass;
	if (!pModel)
	{
		return false;
	}

	result = pModel->Init(pD3D->GetDevice(), L"../common/media/texture/seafloor.dds", "cube.obj");
	
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

	///////////////////////Shader Class ////////////////////////////
	pBitMapShader = new BitMapShader;
	if (!pBitMapShader)
	{
		return false;
	}

	result = pBitMapShader->Init(pD3D->GetDevice(), hWnd);
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
	pLight->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	pLight->SetDirection(0.0f, 0.0f, 1.0f);
	pLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	pLight->SetSpecularPower(32.0f);
	//////////////////////////////////////////////////////////////////

	pBitmap = new BitmapClass;
	if (!pBitmap)
	{
		return false;
	}

	result = pBitmap->Init(pD3D->GetDevice(), screenWidth, screenHeight, L"../common/media/texture/seafloor.dds", 256, 256);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{

	// Release the bitmap object.
	if(pBitmap)
	{
		pBitmap->Shutdown();
		delete pBitmap;
		pBitmap = 0;
	}


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

	// Release the color shader object.
	if(pBitMapShader)
	{
		pBitMapShader->Shutdown();
		delete pBitMapShader;
		pBitMapShader = 0;
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
	rot += (float)D3DX_PI * 0.00005f;
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
	D3DXMATRIX View, Proj, World, Ortho;
	bool result;

	pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
    pCamera->Render();

	pCamera->GetViewMatrix(View);
	pD3D->GetWorld(World);
	pD3D->GetProj(Proj);
	pD3D->GetView(Ortho);

	
	// Turn off the Z buffer to begin all 2D rendering.
	pD3D->TurnZBufferOff();
	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = pBitmap->Render(pD3D->GetDeviceContext(), 100, 100);
	if(!result)
	{
		return false;
	}
	// Render the bitmap with the texture shader.
	result = pBitMapShader->Render(pD3D->GetDeviceContext(), pBitmap->GetIndexCount(), World, View, Ortho, pBitmap->GetTexture() );

	if(!result)
	{
		return false;
	}
	// Turn the Z buffer back on now that all 2D rendering has completed.
	pD3D->TurnZBufferOn();

	
	
	pModel->Render( pD3D->GetDeviceContext() );

	D3DXMatrixRotationY(&World, rot);

	//Render the Model Using Shader
	result = pShader->Render(pD3D->GetDeviceContext(), pModel->GetIndexCount(), World, View, Proj, 
		                     pModel->GetTexture(),  pLight->GetDirection(), pLight->GetAmbientColor(),pLight->GetDiffuseColor(), 
							 pCamera->GetPosition(), pLight->GetSpecularColor(), pLight->GetSpecularPower());
    if (!result)
	{
		return false;
	}

	pD3D->EndScene();
	return true;
}