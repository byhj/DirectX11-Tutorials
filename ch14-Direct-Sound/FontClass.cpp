///////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "FontClass.h"
#include <fstream>

FontClass::FontClass()
{
	pFont = 0;
	pTexture = 0;
}


FontClass::FontClass(const FontClass& other)
{
}


FontClass::~FontClass()
{
}


bool FontClass::Init(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if(!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void FontClass::Shutdown()
{
	// Release the font texture.
	ShutdownTexture();

	// Release the font data.
	ShutdownFontData();

	return;
}


bool FontClass::LoadFontData(char* filename)
{
	std::ifstream fin;
	int i;
	char temp;


	// Create the font spacing buffer.
	pFont = new FontType[95];
	if(!pFont)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if(fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for(i=0; i<95; i++)
	{
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}

		fin >> pFont[i].left;
		fin >> pFont[i].right;
		fin >> pFont[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}


void FontClass::ShutdownFontData()
{
	// Release the font data array.
	if(pFont)
	{
		delete [] pFont;
		pFont = 0;
	}

	return;
}


bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	pTexture = new TextureClass;
	if(!pTexture)
	{
		return false;
	}

	// Initialize the texture object.
	result = pTexture->Init(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void FontClass::ShutdownTexture()
{
	// Release the texture object.
	if(pTexture)
	{
		pTexture->Shutdown();
		delete pTexture;
		pTexture = 0;
	}

	return;
}


ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return pTexture->GetTexture();
}


void FontClass::BuildVAO(void* vertices, char* sentence, float drawX, float drawY)
{
	Vertex* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce the input vertices into a Vertex structure.
	vertexPtr = (Vertex*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	// Draw each letter onto a quad.
	for(i=0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if(letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].Pos = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].Tex = D3DXVECTOR2(pFont[letter].left, 0.0f);
			index++;

			vertexPtr[index].Pos = D3DXVECTOR3((drawX + pFont[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].Tex = D3DXVECTOR2(pFont[letter].right, 1.0f);
			index++;

			vertexPtr[index].Pos = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].Tex = D3DXVECTOR2(pFont[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].Pos = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].Tex = D3DXVECTOR2(pFont[letter].left, 0.0f);
			index++;

			vertexPtr[index].Pos = D3DXVECTOR3(drawX + pFont[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].Tex = D3DXVECTOR2(pFont[letter].right, 0.0f);
			index++;

			vertexPtr[index].Pos = D3DXVECTOR3((drawX + pFont[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].Tex = D3DXVECTOR2(pFont[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + pFont[letter].size + 1.0f;
		}
	}

	return;
}