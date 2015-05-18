#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
	:pD3D(0),
	 pCamera(0),
	 pModel(0),
	 pShader(0),
	 pLight(0),
	 pText(0),
	 pBitMapShader(0),
	 pModelList(0),
	 pFrustum(0)
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

	// Create the text object.
	pText = new TextClass;
	if(!pText)
	{
		return false;
	}

	D3DXMATRIX baseViewMatrix;
	pCamera->Render();
	pCamera->GetViewMatrix(baseViewMatrix);

	// Initialize the text object.
	 result = pText->Init(pD3D->GetDevice(), pD3D->GetDeviceContext(), hWnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	//////////////////////////////////////////////////////////////
	pModelList =  new ModelListClass;
	if (!pModelList)
	{
		return false;
	}

	result = pModelList->Init(25);
	if(!result)
	{
		MessageBox(hWnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}
	//////////////////////////////////////////////////////////////////
	pFrustum = new FrustumClass;
	if (!pFrustum)
	{
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{

	if (!pFrustum)
	{
		delete pFrustum;
		pFrustum = 0;
	}

	if (pModelList)
	{
		pModelList->Shutdown();
		delete pModelList;
		pModelList = 0;
	}

	// Release the text object.
	if(pText)
	{
		pText->Shutdown();
		delete pText;
		pText = 0;
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


bool GraphicsClass::Render(int mouseX, int mouseY, int fps, int cpu , float frameTime, float rotationY)
{
	bool result;

	static float rot = 0.0f;
	rot += (float)D3DX_PI * 0.00005f;
	if (rot > 360.0f)
	{
		rot = 0.0f;
	}

	//pText->SetMousePosition(mouseX, mouseY, pD3D->GetDeviceContext());

	// Set the frames per second.
	result = pText->SetFps(fps, pD3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = pText->SetCpu(cpu, pD3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	result = RenderGraphics(rotationY);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::RenderGraphics(float rot)
{
	D3DXMATRIX View, Proj, World, Ortho;
	bool result, renderModel;
	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	D3DXVECTOR4 color;

	pD3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	pCamera->SetRotation(0.0f, rot, 0.0f);
    pCamera->Render();

	pCamera->GetViewMatrix(View);
	pD3D->GetWorld(World);
	pD3D->GetProj(Proj);
	pD3D->GetView(Ortho);
	pFrustum->ContructFrustum(SCREEN_DEPTH, Proj, View);
	modelCount = pModelList->GetModelCount();
	renderCount = 0;


	// Go through all the models and render them only if they can be seen by the camera view.
	for(index=0; index<modelCount; index++)
	{
		// Get the position and color of the sphere model at this index.
		pModelList->GetData(index, positionX, positionY, positionZ, color);

		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 1.0f;
		// Check if the sphere model is in the view frustum.
		renderModel = pFrustum->CheckSphere(positionX, positionY, positionZ, radius);

		// If it can be seen then render it, if not skip this model and check the next sphere.
		if(renderModel)
		{
			// Move the model to the location it should be rendered at.
			D3DXMatrixTranslation(&World, positionX, positionY, positionZ); 

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			pModel->Render(pD3D->GetDeviceContext());

			// Render the model using the light shader.
			//pShader->Render(pD3D->GetDeviceContext(), pModel->GetIndexCount(), World, View, Proj, 
			//	pModel->GetTexture(), pLight->GetDirection(), color);

			result = pShader->Render(pD3D->GetDeviceContext(), pModel->GetIndexCount(), World, View, Proj, 
				pModel->GetTexture(),  pLight->GetDirection(), pLight->GetAmbientColor(),pLight->GetDiffuseColor(), 
				pCamera->GetPosition(), pLight->GetSpecularColor(), pLight->GetSpecularPower());


			// Reset to the original world matrix.
			pD3D->GetWorld(World);

			// Since this model was rendered then increase the count for this frame.
			renderCount++;
		}
	}
/////////////////////////////////////////////////////////////////////////////////

    pD3D->TurnZBufferOff();
    pD3D->TurnOnAlphaBlending();

	result = pText->SetRenderCount(renderCount, pD3D->GetDeviceContext());

    // Render the text strings.
    result = pText->Render(pD3D->GetDeviceContext(), World, Ortho);
    if(!result)
    {
    	return false;
    }

    pD3D->TurnOffAlphaBlending();
    pD3D->TurnZBufferOn();
/////////////////////////////////////////////////////////////////////////////////

	pD3D->EndScene();
	return true;
}