#ifndef D3DMODEL_H
#define D3DMODEL_H

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "d3dMesh.h"

int TextureFromFile(const char* path, std::string directory);

class D3DModel
{
public:
	D3DModel(){}

	void Render(ID3D11DeviceContext *pD3D11DeviceContext, XMMATRIX model, XMMATRIX view, XMMATRIX proj)
	{
		ModelShader.use(pD3D11DeviceContext);

		pD3D11DeviceContext->PSSetSamplers( 0, 1, &m_pTexSamplerState );

		for (int i = 0; i < this->meshes.size(); i++)
		{
			   float blendFactor[] = {0.4f, 0.4f, 0.4f, 0.3f};
		   if (this->meshes[i].mat.ambient.w < 1.0f)
			 pD3D11DeviceContext->OMSetBlendState(Transparency, blendFactor, 0xffffffff);
		   //"fine-tune" the blending equation
	
			pD3D11DeviceContext->UpdateSubresource(m_pMatBuffer, 0, NULL, &this->meshes[i].mat, 0, 0 );
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &m_pMatBuffer);
			this->meshes[i].Render(pD3D11DeviceContext, model, view, proj);

		   pD3D11DeviceContext->OMSetBlendState(0, 0, 0xffffffff);
		}
	}
	
	//Loading the model form file if support this format
	void loadModel(std::string path);
	void initModel(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
	
   // Processes a node in a recursive fashion. Processes each individual mesh located at 
	//the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);

	//Processes a mesh and return the data information
	D3DMesh processMesh(aiMesh* mesh, const aiScene* scene);
	
	//Load the texture for material
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
	ID3D11ShaderResourceView * TextureFromFile(const char* path, std::string directory);

private:

	//One model may include many meshes
	std::vector<D3DMesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;	

	ID3D11Device *pD3D11Device;
	ID3D11DeviceContext *pD3D11DeviceContext; 
	ID3D11ShaderResourceView *m_pTexture;
	HWND hWnd;
	Shader ModelShader;
	ID3D11SamplerState   *m_pTexSamplerState;
	ID3D11Buffer *m_pMatBuffer;
	ID3D11BlendState* Transparency;
};

#endif