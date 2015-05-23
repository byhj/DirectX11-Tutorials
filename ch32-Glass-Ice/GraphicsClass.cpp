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
	pCamera->SetPosition(0.0f, 0.0f, -10.0f);
	// Create the render to texture object.
	////////////////////////////////////////////////
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


	pModelFloor = new ModelClass;
	if (!pModelFloor)
	{
		return false;
	}

	result = pModelFloor->Init(pD3D->GetDevice(), "floor.txt", L"../common/media/texture/blue01.dds", 
		L"../common/media/texture/bump01.dds", L"../common/media/texture/spec02.dds");

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
	//////////////////////////////////////////////////////////////////////////////
	pReflectionShader = new ReflectioShaderClass;
	if (!pReflectionShader )
	{
		return false;
	}

	result = pReflectionShader->Init(pD3D->GetDevice(), hWnd);
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
		bool result;
		// Render the scene as normal to the back buffer.
	result = RenderToTexture();
		if(!result)
		{
			return false;
		}


	// Render the scene as normal to the back buffer.
	result = RenderScene();
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::RenderToTexture()
{
	D3DXMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix;
	static float rotation = 0.0f;


	// Set the render target to be the render to texture.
	pRenderTexture->SetRenderTarget(pD3D->GetDeviceContext(), pD3D->GetDepthStencilView());

	// Clear the render to texture.
	pRenderTexture->ClearRenderTarget(pD3D->GetDeviceContext(), pD3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Use the camera to calculate the reflection matrix.
	pCamera->RenderReflection(-1.5f);

	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = pCamera->GetReflectionViewMatrix();

	// Get the world and projection matrices.
	pD3D->GetWorld(worldMatrix);
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

	// Render the model using the texture shader and the reflection view matrix.
	pShader->Render(pD3D->GetDeviceContext(), pModel->GetIndexCount(), worldMatrix, reflectionViewMatrix, 
		projectionMatrix, pModel->GetTextureArray(),
		pLight->GetDirection(), pLight->GetDiffuseColor(),
		pCamera->GetPosition(), pLight->GetSpecularColor(), pLight->GetSpecularPower());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	pD3D->SetBackBufferRenderTarget();


	return true;
}


bool GraphicsClass::RenderScene()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix;
	bool result;
	static float rotation = 0.0f;


	// Clear the buffers to begin the scene.
	pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

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

	// Multiply the world matrix by the rotation.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	pModel->Render(pD3D->GetDeviceContext());

	// Render the model with the texture shader.
	result = pShader->Render(pD3D->GetDeviceContext(), pModel->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, pModel->GetTextureArray(), pLight->GetDirection(), pLight->GetDiffuseColor(),
		pCamera->GetPosition(), pLight->GetSpecularColor(), pLight->GetSpecularPower() );
	if(!result)
	{
		return false;
	}

	// Get the world matrix again and translate down for the floor model to render underneath the cube.
	pD3D->GetWorld(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 0.0f, -1.5f, -1.5f); 

	// Get the camera reflection view matrix.
	reflectionMatrix = pCamera->GetReflectionViewMatrix();

	// Put the floor model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	pModelFloor->Render(pD3D->GetDeviceContext());

	// Render the floor model using the reflection shader, reflection texture, and reflection view matrix.
	result = pReflectionShader->Render(pD3D->GetDeviceContext(), pModelFloor->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, pModelFloor->GetTexture(), pRenderTexture->GetShaderResourceView(), 
		reflectionMatrix);
	if(!result)
	{
		return false;
	}
	// Present the rendered scene to the screen.
	pD3D->EndScene();

	return true;
}