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

	//////////////////////// Model Class ////////////////////////////
	pModel = new ModelClass;
	if (!pModel)
	{
		return false;
	}

	result = pModel->Init(pD3D->GetDevice(), L"../common/media/texture/grid01.dds", "floor.txt");
	
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	///////////////////////////////////////////////////////////////
	// Create the billboard model object.
	m_BillboardModel = new ModelClass;
	if(!m_BillboardModel)
	{
		return false;
	}

	// Initialize the billboard model object.
	result = m_BillboardModel->Init(pD3D->GetDevice(),  L"../common/media/texture/seafloor.dds", "square.txt");
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the billboard model object.", L"Error", MB_OK);
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
	pLight->SetDirection(0.0, 0.0, 1.0f);

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


bool GraphicsClass::Render(float positionX, float positionY, float positionZ)
{
	bool result;
	pCamera->SetPosition(positionX, positionY, positionZ);
	static float rot = 0.0f;
	rot += (float)D3DX_PI * 0.0005f;
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
	bool result;
	D3DXVECTOR3 cameraPosition, modelPosition;
	double angle;
	float rotation;


	// Clear the buffers to begin the scene.
	pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	pCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	pCamera->GetViewMatrix(viewMatrix);
	pD3D->GetWorld(worldMatrix);
	pD3D->GetProj(projectionMatrix);
		// Put the floor model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	pModel->Render(pD3D->GetDeviceContext());
	// Render the floor model using the texture shader.
	result = pShader->Render(pD3D->GetDeviceContext(), pModel->GetIndexCount(), worldMatrix, viewMatrix, 
		                     projectionMatrix,  pModel->GetTexture(), pLight->GetDirection(), pLight->GetDiffuseColor());
	if(!result)
	{
		return false;
	}
	

	// Get the position of the camera.
	cameraPosition = pCamera->GetPosition();

	// Set the position of the billboard model.
	modelPosition.x = 0.0f;
	modelPosition.y = 1.5f;
	modelPosition.z = 0.0f;
	
   // Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / D3DX_PI);

	// Convert rotation into radians.
	rotation = (float)angle * 0.0174532925f;

		// Setup the rotation the billboard at the origin using the world matrix.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Setup the translation matrix from the billboard model.
	D3DXMatrixTranslation(&translateMatrix, modelPosition.x, modelPosition.y, modelPosition.z);

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix); 


		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_BillboardModel->Render(pD3D->GetDeviceContext());


	//Render the Model Using Shader
	result = pShader->Render(pD3D->GetDeviceContext(), pModel->GetIndexCount(),  worldMatrix, viewMatrix, 
		                     projectionMatrix, pModel->GetTexture(), pLight->GetDirection(), pLight->GetDiffuseColor() );
    if (!result)
	{
		return false;
	}

	pD3D->EndScene();
	return true;
}