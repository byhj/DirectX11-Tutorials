#include "d3dModel.h"


void D3DModel::initModel(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
{
	this->pD3D11Device = pD3D11Device;
	this->pD3D11DeviceContext = pD3D11DeviceContext;
	this->hWnd = hWnd;
	init_shader(pD3D11Device, hWnd);

	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc;
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

	HRESULT hr;
	// Create the texture sampler state.
	hr = pD3D11Device->CreateSamplerState(&samplerDesc, &m_pTexSamplerState);
	DebugHR(hr);

	///////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC cbMaterialDesc;	
	ZeroMemory(&cbMaterialDesc, sizeof(D3D11_BUFFER_DESC));
	cbMaterialDesc.Usage          = D3D11_USAGE_DEFAULT;
	cbMaterialDesc.ByteWidth      = sizeof(Material);
	cbMaterialDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	cbMaterialDesc.CPUAccessFlags = 0;
	cbMaterialDesc.MiscFlags      = 0;
	hr = pD3D11Device->CreateBuffer(&cbMaterialDesc, NULL, &m_pMatBuffer);
	DebugHR(hr);

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof(blendDesc) );
	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory( &rtbd, sizeof(rtbd) );
	rtbd.BlendEnable			 = true;
	rtbd.SrcBlend				 = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend				 = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp				 = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha			 = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha			 = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask	 = D3D10_COLOR_WRITE_ENABLE_ALL;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;
	pD3D11Device->CreateBlendState(&blendDesc, &Transparency);
}

void D3DModel::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
{
	HRESULT hr;
	D3D11_INPUT_ELEMENT_DESC pInputLayoutDesc[3];

	pInputLayoutDesc[0].SemanticName         = "POSITION";
	pInputLayoutDesc[0].SemanticIndex        = 0;
	pInputLayoutDesc[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[0].InputSlot            = 0;
	pInputLayoutDesc[0].AlignedByteOffset    = 0;
	pInputLayoutDesc[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[0].InstanceDataStepRate = 0;

	pInputLayoutDesc[1].SemanticName         = "NORMAL";
	pInputLayoutDesc[1].SemanticIndex        = 0;
	pInputLayoutDesc[1].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	pInputLayoutDesc[1].InputSlot            = 0;
	pInputLayoutDesc[1].AlignedByteOffset    = 12;
	pInputLayoutDesc[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[1].InstanceDataStepRate = 0;

	pInputLayoutDesc[2].SemanticName         = "TEXCOORD";
	pInputLayoutDesc[2].SemanticIndex        = 0;
	pInputLayoutDesc[2].Format               = DXGI_FORMAT_R32G32_FLOAT;
	pInputLayoutDesc[2].InputSlot            = 0;
	pInputLayoutDesc[2].AlignedByteOffset    = 24;
	pInputLayoutDesc[2].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	pInputLayoutDesc[2].InstanceDataStepRate = 0;

	unsigned numElements = ARRAYSIZE(pInputLayoutDesc);

	ModelShader.init(pD3D11Device, hWnd);
	ModelShader.attachVS(L"model.vsh", pInputLayoutDesc, numElements);
	ModelShader.attachPS(L"model.psh");
	ModelShader.end();
}

void D3DModel::loadModel(std::string path)
{
	// Read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	// Retrieve the directory path of the filepath
	this->directory = path.substr(0, path.find_last_of('/'));

	// Process ASSIMP's root node recursively
	this->processNode(scene->mRootNode, scene);
}

std::vector<Texture>  D3DModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (int j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			Texture texture;
			texture.pTexture = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

void setColor(aiColor3D &c, XMFLOAT4 &m)
{
	m.x = c[0];
	m.y = c[1];
	m.z = c[2];
}

void setBlend(float blend, Material &mat)
{
	mat.ambient.w = blend;
	mat.specular.w = blend;
	mat.diffuse.w = blend;
	mat.emissive.w = blend;
}

D3DMesh D3DModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	//Mesh Data to fill
	std::vector<Vertex> vertices;
	std::vector<unsigned long> indices;
	std::vector<Texture> textures;
	std::vector<Material> MaterialData;

	// Walk through each of the mesh's vertices
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		XMFLOAT3 temp; // We declare a placeholder std::vector since assimp uses its own std::vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder XMFloat3 first.

		// Positions
		temp.x = mesh->mVertices[i].x;
		temp.y = mesh->mVertices[i].y;
		temp.z = mesh->mVertices[i].z;
		vertex.Position = temp;

		// Normals
		temp.x = mesh->mNormals[i].x;
		temp.y = mesh->mNormals[i].y;
		temp.z = mesh->mNormals[i].z;
		vertex.Normal = temp;

		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			XMFLOAT2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = XMFLOAT2(0.0f, 0.0f);

		//Process one Vertex
		vertices.push_back(vertex);
	}

	// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices std::vector
		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	Material mat;

	// Process materials
	if (mesh->mMaterialIndex >= 0)
	{
		int tt = mesh->mMaterialIndex;
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		aiColor3D ambient;
		if(AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, ambient) )
			setColor(ambient, mat.ambient);
		//std::cout << mat.ambient.w << std::endl;
		aiColor3D diffuse;
		if(AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) )
			setColor(diffuse, mat.diffuse);

		aiColor3D  speucular;
		if(AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, speucular) )
			setColor(speucular, mat.specular);

		aiColor3D  emissive;
		if(AI_SUCCESS == material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive) )
			setColor(emissive, mat.emissive);

		//float shininess = 0.0;
		//if(AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess))
		//	mat.shininess = shininess;

		float blend;
		material->Get(AI_MATKEY_OPACITY , blend);
		if (blend < 1.0f)
			setBlend(blend, mat);
		//std::cout << mat.ambient.w << std::endl;

		// 1. Diffuse maps
		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// 2. Specular maps
		std::vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	// Return a mesh object created from the extracted mesh data
	return D3DMesh(vertices, indices, textures, mat, pD3D11Device, pD3D11DeviceContext, hWnd);
}

void D3DModel::processNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh located at the current node
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}

	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (int i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}

}

ID3D11ShaderResourceView * D3DModel::TextureFromFile(const char* path, std::string directory)
{
	//Generate texture ID and load texture data 
	std::string filename = std::string(path);
	std::string texFile = directory + '/' + filename;

	std::wstring stemp = std::wstring(texFile.begin(), texFile.end());
	LPCWSTR sw = stemp.c_str();

	HRESULT hr;
	hr = D3DX11CreateShaderResourceViewFromFile(pD3D11Device, sw, NULL,NULL, &m_pTexture, NULL);
	DebugHR(hr);
	return m_pTexture;
}
