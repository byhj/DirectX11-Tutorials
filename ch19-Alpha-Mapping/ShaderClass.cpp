#include "ShaderClass.h"

ShaderClass::ShaderClass()
	:pVS_Shader(0),
	 pPS_Shader(0),
	 pInputLayout(0),
	 pMatrixBuffer(0),
	 pSampleState(0)
{

}

ShaderClass::ShaderClass(const ShaderClass &)
{

}

ShaderClass::~ShaderClass()
{

}

bool ShaderClass::Init(ID3D11Device *pD3D11Device, HWND hwnd)
{
	bool result;
	result = InitShader(pD3D11Device, hwnd, L"texture-vs.hlsl", L"texture-fs.hlsl");

	if (!result)
	{
        return false;
	}
	return true;
}

void ShaderClass::Shutdown()
{
	ShutdownShader();

	return ;
}

bool ShaderClass::Render(ID3D11DeviceContext *pD3D11DeviceContext, int IndexCount
						 ,D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, 
						 ID3D11ShaderResourceView** textureArray)
{
	bool result;
	result = SetShaderParameters(pD3D11DeviceContext, World, View, Proj, textureArray);
	if(!result)
	{
		return false;
	}

	RenderShader(pD3D11DeviceContext, IndexCount);
	return true;
}

bool ShaderClass::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
		&vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
		&pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &pVS_Shader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pPS_Shader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), &pInputLayout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &pMatrixBuffer);
	if(FAILED(result))
	{
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &pSampleState);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}



void ShaderClass::ShutdownShader()
{

	// Release the sampler state.
	if(pSampleState)
	{
		pSampleState->Release();
		pSampleState = 0;
	}

	if (pMatrixBuffer)
	{
		pMatrixBuffer->Release();
		pMatrixBuffer = 0;
	}
	if (pInputLayout)
	{
		pInputLayout->Release();
		pInputLayout = 0;
	}
	if (pPS_Shader)
	{
		pPS_Shader->Release();
		pPS_Shader = 0;
	}
	if (pVS_Shader)
	{
		pVS_Shader->Release();
		pVS_Shader = 0;
	}

	return ;
}

void ShaderClass::OutputShaderErrorMessage(ID3D10Blob *pErrorMessage, HWND hwnd, WCHAR *shaderFileName)
{
	char *pCompileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;

	pCompileErrors = (char*)(pErrorMessage->GetBufferPointer());
	bufferSize = pErrorMessage->GetBufferSize();

	fout.open("shader-error.txt");
	for (i = 0; i < bufferSize; ++i)
	{
		fout << pCompileErrors[i];
	}

	fout.close();
	pErrorMessage->Release();
	pErrorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFileName, MB_OK);

	return;

}

bool ShaderClass::SetShaderParameters(ID3D11DeviceContext* pD3D11DeviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
										   D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView **textureArray)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBuffer* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

		// Lock the constant buffer so it can be written to.
	result = pD3D11DeviceContext->Map(pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBuffer*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->World = worldMatrix;
	dataPtr->View = viewMatrix;
	dataPtr->Proj = projectionMatrix;

	// Unlock the constant buffer.
	pD3D11DeviceContext->Unmap(pMatrixBuffer, 0);
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	pD3D11DeviceContext->VSSetConstantBuffers(bufferNumber, 1, &pMatrixBuffer);

	//Two texture view
	pD3D11DeviceContext->PSSetShaderResources(0, 3, textureArray);
	return true;
}


void ShaderClass::RenderShader(ID3D11DeviceContext *pD3D11DeviceContext, int IndexCount)
{
	pD3D11DeviceContext->IASetInputLayout(pInputLayout);
	pD3D11DeviceContext->VSSetShader(pVS_Shader, NULL, 0);
	pD3D11DeviceContext->PSSetShader(pPS_Shader, NULL, 0);

	// Set the sampler state in the pixel shader.
	pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampleState);

	pD3D11DeviceContext->DrawIndexed(IndexCount, 0, 0);
}