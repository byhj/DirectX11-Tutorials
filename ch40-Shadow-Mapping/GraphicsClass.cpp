#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
	:pD3D(0),
	 pCamera(0),
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
	result = pRenderTexture->Init(pD3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		return false;
	}
	//////////////////////// Model Class ////////////////////////////
	// Create the cube model object.
	pCubeModel = new ModelClass;
	if(!pCubeModel)
	{
		return false;
	}

	// Initialize the cube model object.
	result = pCubeModel->Init(pD3D->GetDevice(), L"../common/media/texture/wall01.dds", "cube.txt");
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the cube model object.", L"Error", MB_OK);
		return false;
	}
	// Set the position for the cube model.
	pCubeModel->SetPosition(-2.0f, 2.0f, 0.0f);

	// Create the sphere model object.
	pSphereModel = new ModelClass;
	if(!pSphereModel)
	{
		return false;
	}

	// Initialize the sphere model object.
	result = pSphereModel->Init(pD3D->GetDevice(), L"../common/media/texture/ice.dds",  "sphere.txt");
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the sphere model object.", L"Error", MB_OK);
		return false;
	}

	// Set the position for the sphere model.
	pSphereModel->SetPosition(2.0f, 2.0f, 0.0f);

	// Create the ground model object.
	pGroundModel = new ModelClass;
	if(!pGroundModel)
	{
		return false;
	}

	// Initialize the ground model object.
	result = pGroundModel->Init(pD3D->GetDevice(), L"../common/media/texture/metal001.dds" , "plane01.txt");
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the ground model object.", L"Error", MB_OK);
		return false;
	}

	// Set the position for the ground model.
	pGroundModel->SetPosition(0.0f, 1.0f, 0.0f);
	///////////////////////Shader Class ////////////////////////////
	pShadowShader = new ShadowShaderClass;
	if (!pShadowShader)
	{
       return false;
	}

	result = pShadowShader->Init(pD3D->GetDevice(), hWnd);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////
	// Create the depth shader object.
	pDepthShader = new DepthShaderClass;
	if(!pDepthShader)
	{
		return false;
	}

	// Initialize the depth shader object.
	result = pDepthShader->Init(pD3D->GetDevice(), hWnd);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the depth shader object.", L"Error", MB_OK);
		return false;
	}
	pLight = new LightClass;
	if (!pLight)
	{
		return false;
	}

	pLight->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	pLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	pLight->SetLookAt(0.0f, 0.0f, 0.0f);
	pLight->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	// Create the render to texture object.
	pRenderTexture = new RenderTextureClass;
	if(!pRenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = pRenderTexture->Init(pD3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}
	return true;
}


void GraphicsClass::Shutdown()
{
	if (pLight)
	{
		delete pLight;
		pLight = 0;
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
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;
	D3DXMATRIX lightViewMatrix, lightProjectionMatrix;
	bool result;
	float posX, posY, posZ;


	// First render the scene to a texture.
	result = RenderToTexture();
	if(!result)
	{
		return false;
	}

	// Clear the buffers to begin the scene.
	pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	pCamera->Render();

	// Generate the light view matrix based on the light's position.
	pLight->GenerateViewMatrix();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	pCamera->GetViewMatrix(viewMatrix);
	pD3D->GetWorld(worldMatrix);
	pD3D->GetProj(projectionMatrix);

	// Get the light's view and projection matrices from the light object.
	pLight->GetViewMatrix(lightViewMatrix);
	pLight->GetProjectionMatrix(lightProjectionMatrix);

	// Setup the translation matrix for the cube model.
	pCubeModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Put the cube model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	pCubeModel->Render(pD3D->GetDeviceContext());

	// Render the model using the shadow shader.
	result = pShadowShader->Render(pD3D->GetDeviceContext(), pCubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, 
		lightProjectionMatrix, pCubeModel->GetTexture(), pRenderTexture->GetShaderResourceView(), pLight->GetPosition(),
		pLight->GetAmbientColor(), pLight->GetDiffuseColor());
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	pD3D->GetWorld(worldMatrix);

	// Setup the translation matrix for the sphere model.
	pSphereModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	pSphereModel->Render(pD3D->GetDeviceContext());
	result = pShadowShader->Render(pD3D->GetDeviceContext(), pSphereModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, 
		lightProjectionMatrix, pSphereModel->GetTexture(), pRenderTexture->GetShaderResourceView(), pLight->GetPosition(), 
		pLight->GetAmbientColor(), pLight->GetDiffuseColor());
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	pD3D->GetWorld(worldMatrix);

	// Setup the translation matrix for the ground model.
	pGroundModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the ground model using the shadow shader.
	pGroundModel->Render(pD3D->GetDeviceContext());
	result = pShadowShader->Render(pD3D->GetDeviceContext(), pGroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
		lightProjectionMatrix, pGroundModel->GetTexture(), pRenderTexture->GetShaderResourceView(), pLight->GetPosition(), 
		pLight->GetAmbientColor(), pLight->GetDiffuseColor());
	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	pD3D->EndScene();


	return true;
}

bool GraphicsClass::RenderToTexture()
{
	D3DXMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, translateMatrix;
	float posX, posY, posZ;
	bool result;


	// Set the render target to be the render to texture.
	pRenderTexture->SetRenderTarget(pD3D->GetDeviceContext());

	// Clear the render to texture.
	pRenderTexture->ClearRenderTarget(pD3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the light view matrix based on the light's position.
	pLight->GenerateViewMatrix();

	// Get the world matrix from the d3d object.
	pD3D->GetWorld(worldMatrix);

	// Get the view and orthographic matrices from the light object.
	pLight->GetViewMatrix(lightViewMatrix);
	pLight->GetProjectionMatrix(lightProjectionMatrix);

	// Setup the translation matrix for the cube model.
	pCubeModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the cube model with the depth shader.
	pCubeModel->Render(pD3D->GetDeviceContext());
	result = pDepthShader->Render(pD3D->GetDeviceContext(), pCubeModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	pD3D->GetWorld(worldMatrix);

	// Setup the translation matrix for the sphere model.
	pSphereModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the sphere model with the depth shader.
	pSphereModel->Render(pD3D->GetDeviceContext());
	result = pDepthShader->Render(pD3D->GetDeviceContext(), pSphereModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if(!result)
	{
		return false;
	}

	// Reset the world matrix.
	pD3D->GetWorld(worldMatrix);

	// Setup the translation matrix for the ground model.
	pGroundModel->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the ground model with the depth shader.
	pGroundModel->Render(pD3D->GetDeviceContext());
	result = pDepthShader->Render(pD3D->GetDeviceContext(), pGroundModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if(!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	pD3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	pD3D->ResetViewport();

	return true;
}

