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
	pCamera->SetPosition(0.0f, 7.0f, -10.0f);
	pCamera->SetRotation(35.0f, 0.0f, 0.0f);
	// Create the render to texture object.

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


	// Create the ground model object.
	pGroundModel = new ModelClass;
	if(!pGroundModel)
	{
		return false;
	}

	// Initialize the ground model object.
	result = pGroundModel->Init(pD3D->GetDevice(), L"../common/media/texture/metal001.dds" , "floor.txt");
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
	pLight = new LightClass;
	if (!pLight)
	{
		return false;
	}

	pLight->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	pLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	pLight->SetPosition(2.0f, 5.0f, -2.0f);
	
	// Create the view point object.
	m_ViewPoint = new ViewPointClass;
	if(!m_ViewPoint)
	{
		return false;
	}

	// Initialize the view point object.
	m_ViewPoint->SetPosition(2.0f, 5.0f, -2.0f);
	m_ViewPoint->SetLookAt(0.0f, 0.0f, 0.0f);
	m_ViewPoint->SetProjectionParameters((float)(D3DX_PI / 2.0f), 1.0f, 0.1f, 100.0f);
	m_ViewPoint->GenerateViewMatrix();
	m_ViewPoint->GenerateProjectionMatrix();

	// Create the projection texture object.
	m_ProjectionTexture = new TextureClass;
	if(!m_ProjectionTexture)
	{
		return false;
	}

	// Initialize the projection texture object.
	result = m_ProjectionTexture->Init(pD3D->GetDevice(), L"../common/media/texture/grate.dds");
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the projection texture object.", L"Error", MB_OK);
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
	rot += (float)D3DX_PI * 0.0001f;
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
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	D3DXMATRIX viewMatrix2, projectionMatrix2;
	bool result;


	// Clear the buffers to begin the scene.
	pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	pCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	pD3D->GetWorld(worldMatrix);
	pCamera->GetViewMatrix(viewMatrix);
	pD3D->GetProj(projectionMatrix);

		// Get the view and projection matrices from the view point object.
	m_ViewPoint->GetViewMatrix(viewMatrix2);
	m_ViewPoint->GetProjectionMatrix(projectionMatrix2);

		// Setup the translation for the ground model.
	D3DXMatrixTranslation(&worldMatrix, 0.0f, 1.0f, 0.0f);

	// Render the ground model using the projection shader.
	pGroundModel->Render(pD3D->GetDeviceContext());
	result = pShadowShader->Render(pD3D->GetDeviceContext(), pGroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		pGroundModel->GetTexture(), pLight->GetAmbientColor(), pLight->GetDiffuseColor(), pLight->GetDirection(), 
		viewMatrix2, projectionMatrix2, m_ProjectionTexture->GetTexture(), pLight->GetPosition());
	if(!result)
	{
		return false;
	}

	// Reset the world matrix and setup the translation for the cube model.
	pD3D->GetWorld(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 0.0f, 2.0f, 0.0f);
	D3DXMATRIX rm;
	D3DXMatrixRotationY(&rm, rot);
	worldMatrix *= rm;
	// Render the cube model using the projection shader.
	pCubeModel->Render(pD3D->GetDeviceContext());
	result = pShadowShader->Render(pD3D->GetDeviceContext(), pCubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		pCubeModel->GetTexture(), pLight->GetAmbientColor(), pLight->GetDiffuseColor(), pLight->GetDirection(), 
		viewMatrix2, projectionMatrix2,  m_ProjectionTexture->GetTexture(), pLight->GetPosition());
	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	pD3D->EndScene();

	return true;
}
