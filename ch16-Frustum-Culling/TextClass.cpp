#include "TextClass.h"

TextClass::TextClass()
	:pFont(0),
	 pFontShader(0),
	 pSentence1(0),
	 pSentence2(0)
{

}

TextClass::TextClass(const TextClass &rhs)
{

}

TextClass::~TextClass()
{
}

bool TextClass::Init(ID3D11Device* pD3D11Device, ID3D11DeviceContext* pD3D11DeviceContext, HWND hwnd, int ScreenWidth, int ScreenHeight, 
					 D3DXMATRIX baseViewMatrix)
{
	bool result;
	screenWidth = ScreenWidth;
	screenHeight = ScreenHeight;
	BaseView = baseViewMatrix;

	pFont = new FontClass;
	if (!pFont)
	{
		return false;
	}
	result = pFont->Init(pD3D11Device, "fontdata.txt", L"font.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

    pFontShader = new FontShader;
	if (!pFontShader)
	{
		return false;
	}
    result = pFontShader->Init(pD3D11Device, hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the first sentence.
	result = InitBuffer(&pSentence1, 16, pD3D11Device);
	if(!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
	result = UpdateBuffer(pSentence1, "Hello", 100, 100, 1.0f, 0.0f, 0.0f, pD3D11DeviceContext);
	if(!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitBuffer(&pSentence2, 16, pD3D11Device);
	if(!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information.
    result = UpdateBuffer(pSentence2, "Hello", 100, 200, 1.0f, 1.0f, 1.0f, pD3D11DeviceContext);
	if(!result)
	{
		return false;
	}
	return true;
}

void TextClass::Shutdown()
{
	// Release the first sentence.
	ShutdownBuffer(&pSentence1);

	// Release the second sentence.
	ShutdownBuffer(&pSentence2);

	// Release the font shader object.
	if(pFontShader)
	{
		pFontShader->Shutdown();
		delete pFontShader;
		pFontShader = 0;
	}

	// Release the font object.
	if(pFont)
	{
		pFont->Shutdown();
		delete pFont;
		pFont = 0;
	}

	return;
}

bool TextClass::Render(ID3D11DeviceContext *pD3D11DeviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{	
	bool result;


	// Draw the first sentence.
	result = RenderBuffer(pD3D11DeviceContext, pSentence1, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Draw the second sentence.
	result = RenderBuffer(pD3D11DeviceContext, pSentence2, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char fpsString[16];
	float red, green, blue;
	bool result;


	// Truncate the fps to below 10,000.
	if(fps > 9999)
	{
		fps = 9999;
	}

	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup the fps string.
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	// If fps is 60 or above set the fps color to green.
	if(fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if(fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if(fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	result = UpdateBuffer(pSentence1, fpsString, 20, 20, red, green, blue, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}
bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;


	// Convert the cpu integer to string format.
	_itoa_s(cpu, tempString, 10);

	// Setup the cpu string.
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// Update the sentence vertex buffer with the new string information.
	result = UpdateBuffer(pSentence2, cpuString, 20, 40, 0.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetRenderCount(int count, ID3D11DeviceContext* deviceContext)
{
	char tempString[32];
	char countString[32];
	bool result;


	// Convert the count integer to string format.
	_itoa_s(count, tempString, 10);

	// Setup the render count string.
	strcpy_s(countString, "Render Count: ");
	strcat_s(countString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateBuffer(pSentence2, countString, 20, 40, 1.0f, 1.0f, 1.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char mouseString[16];
	bool result;


	// Convert the mouseX integer to string format.
	_itoa_s(mouseX, tempString, 10);

	// Setup the mouseX string.
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateBuffer(pSentence1, mouseString, 10, 10, 1.0f, 1.0f, 1.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	// Convert the mouseY integer to string format.
	_itoa_s(mouseY, tempString, 10);

	// Setup the mouseY string.
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateBuffer(pSentence2, mouseString, 10, 30, 1.0f, 1.0f, 1.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}

bool TextClass::InitBuffer(Sentence** sentence, int maxLength, ID3D11Device* pD3D11Device)
{
	Vertex *VertexData;
	unsigned long *IndexData;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VBO, IBO;
	HRESULT result;


	// Create a new sentence object.
	*sentence = new Sentence;
	if(!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	VertexData = new Vertex[(*sentence)->vertexCount];
	if(!VertexData)
	{
		return false;
	}

	// Create the index array.
	IndexData = new unsigned long[(*sentence)->indexCount];
	if(!IndexData)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(VertexData, 0, (sizeof(Vertex) * (*sentence)->vertexCount));

	// Initialize the index array.
	for(int i=0; i<(*sentence)->indexCount; i++)
	{
		IndexData[i] = i;
	}
	/////////////////////////////////////////////////////////////////
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(Vertex) * (*sentence)->vertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	VBO.pSysMem = VertexData;
	VBO.SysMemPitch = 0;
	VBO.SysMemSlicePitch = 0;

	result = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//////////////////////////////////////////////////////
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	IBO.pSysMem = IndexData;
	IBO.SysMemPitch = 0;
	IBO.SysMemSlicePitch = 0;
	result = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete VertexData;
	VertexData = 0;
	delete IndexData;
	IndexData = 0;

	return true;
}

void TextClass::ShutdownBuffer(Sentence** sentence)
{
		if(*sentence)
		{
			// Release the sentence vertex buffer.
			if((*sentence)->vertexBuffer)
			{
				(*sentence)->vertexBuffer->Release();
				(*sentence)->vertexBuffer = 0;
			}

			// Release the sentence index buffer.
			if((*sentence)->indexBuffer)
			{
				(*sentence)->indexBuffer->Release();
				(*sentence)->indexBuffer = 0;
			}

			// Release the sentence.
			delete *sentence;
			*sentence = 0;
		}

		return;
}

bool TextClass::UpdateBuffer(Sentence* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
	                          ID3D11DeviceContext* pD3D11DeviceContext)
{
	int numLetters;
	Vertex* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Vertex* verticesPtr;

	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new Vertex[sentence->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(Vertex) * sentence->vertexCount));


		// Calculate the X and Y pixel position on the screen to start drawing to.
		drawX = (float)(((screenWidth / 2) * -1) + positionX);
	drawY = (float)((screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	pFont->BuildVAO((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = pD3D11DeviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (Vertex*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(Vertex) * sentence->vertexCount));

	// Unlock the vertex buffer.
	pD3D11DeviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}



bool TextClass::RenderBuffer(ID3D11DeviceContext* deviceContext, Sentence* sentence, D3DXMATRIX worldMatrix, 
							   D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;


	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = pFontShader->Render(deviceContext, sentence->indexCount, worldMatrix, BaseView, orthoMatrix, pFont->GetTexture(), 
		pixelColor);
	if(!result)
	{
		false;
	}

	return true;
}