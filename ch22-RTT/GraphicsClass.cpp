#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
	:pD3D(0),
	 pCamera(0),
	 pModel(0),
	 pShader(0),
	 pLight(0),
	 pDebugWindow(0),
	 pRenderTexture(0),
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

//////////////////////////////////////////////////////////////////////

	pLight = new LightClass;
	if (!pLight)
	{
		return false;
	}

	pLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	pLight->SetDirection(0.0f, 0.0f, 1.0f);
	pLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	pLight->SetSpecularPower(16.0f);


	// Create the render to texture object.
	pRenderTexture = new RenderTextureClass;
	if(!pRenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = pRenderTexture->Init(pD3D->GetDevice(), screenWidth, screenHeight);
	if(!result)
	{
		return false;
	}


	/////////////////////////////////////////////////////////////////////////
	// Create the debug window object.
	pDebugWindow = new DebugWindowClass;
	if(!pDebugWindow)
	{
		return false;
	}

	// Initialize the debug window object.
	result = pDebugWindow->Init(pD3D->GetDevice(), screenWidth, screenHeight, 200, 200);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}
	return true;

}


void GraphicsClass::Shutdown()
{
	// Release the color shader object.
	if(pBitMapShader)
	{
		pBitMapShader->Shutdown();
		delete pBitMapShader;
		pBitMapShader = 0;
	}

	// Release the debug window object.
	if(pDebugWindow)
	{
		pDebugWindow->Shutdown();
		delete pDebugWindow;
		pDebugWindow = 0;
	}

	// Release the render to texture object.
	if(pRenderTexture)
	{
		pRenderTexture->Shutdown();
		delete pRenderTexture;
		pRenderTexture = 0;
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
	// Render the entire scene to the texture first.
	result = RenderToTexture();
	if(!result)
	{
		return false;
	}
	// Turn off the Z buffer to begin all 2D rendering.

	pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	// Render the scene as normal to the back buffer.
	result = RenderScene();
	if(!result)
	{
		return false;
	}

	pD3D->TurnZBufferOff();

	pCamera->GetViewMatrix(View);
	pD3D->GetWorld(World);
	pD3D->GetProj(Proj);
	pD3D->GetView(Ortho);

	// Put the debug window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = pDebugWindow->Render(pD3D->GetDeviceContext(), 0, 0);
	if(!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = pBitMapShader->Render(pD3D->GetDeviceContext(), pDebugWindow->GetIndexCount(), World, View, Ortho, pRenderTexture->GetShaderResourceView());
	// Render the debug window using the texture shader.

    if (!result)
	{
		return false;
	}
	// Turn the Z buffer back on now that all 2D rendering has completed.
	pD3D->TurnZBufferOn();

	pD3D->EndScene();
	return true;
}

bool GraphicsClass::RenderToTexture()
{
	bool result;

	// Set the render target to be the render to texture.
	pRenderTexture->SetRenderTarget(pD3D->GetDeviceContext(), pD3D->GetDepthStencilView());

		// Clear the render to texture.
	pRenderTexture->ClearRenderTarget(pD3D->GetDeviceContext(), pD3D->GetDepthStencilView(), 0.5f, 0.5f, 0.5f, 1.0f);

	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	result = RenderScene();
	if(!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	pD3D->SetBackBufferRenderTarget();

	return result;
}


bool GraphicsClass::RenderScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	static float rotation = 0.0f;


	// Generate the view matrix based on the camera's position.
	pCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	pD3D->GetWorld(worldMatrix);
	pCamera->GetViewMatrix(viewMatrix);
	pD3D->GetProj(projectionMatrix);

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.00005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	pModel->Render(pD3D->GetDeviceContext());

	// Render the model using the light shader.
	result = pShader->Render(pD3D->GetDeviceContext(), pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		pModel->GetTextureArray(), pLight->GetDirection(), pLight->GetDiffuseColor(),
		pCamera->GetPosition(), pLight->GetSpecularColor(), pLight->GetSpecularPower() );

	if(!result)
	{
		return false;
	}

	return result;
}