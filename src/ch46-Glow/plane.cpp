#include "Plane.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{
	void Plane::Init(ID3D11Device *pD3D11Device, int width, int height)
	{
		m_width = width;
		m_height = height;
		init_buffer(pD3D11Device);
	}

	void Plane::Render(ID3D11DeviceContext *pD3D11DeviceContext)
	{

		unsigned int stride;
		unsigned int offset;
		stride = sizeof(Vertex);
		offset = 0;


		pD3D11DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		pD3D11DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pD3D11DeviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}

	void Plane::Clear(ID3D11DeviceContext *pD3D11DeviceContext)
	{
		pD3D11DeviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

		float color[4] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		pD3D11DeviceContext->ClearRenderTargetView(m_renderTargetView, color);
		pD3D11DeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	    pD3D11DeviceContext->RSSetViewports(1, &m_viewport);
	}



	void Plane::Shutdown()
	{
		ReleaseCOM(m_pVertexBuffer)
		ReleaseCOM(m_pIndexBuffer)
	}


	void Plane::init_buffer(ID3D11Device *pD3D11Device)
	{
		HRESULT hr;

		m_VertexCount = 6;
		m_IndexCount = 6;

		std::vector<Vertex> VertexData(6);
		std::vector<UINT> IndexData(6);

		for (int i = 0; i != m_IndexCount; ++i)
			IndexData[i] = i;

		float left, right, top, bottom;
		// Calculate the screen coordinates of the left side of the window.
		left = (float)((m_width / 2) * -1);

		// Calculate the screen coordinates of the right side of the window.
		right = left + (float)m_width;

		// Calculate the screen coordinates of the top of the window.
		top = (float)(m_height / 2);

		// Calculate the screen coordinates of the bottom of the window.
		bottom = top - (float)m_height;

		// First triangle.
		VertexData[0].Pos = XMFLOAT3(left, top, 0.0f);  // Top left.
		VertexData[0].Tex = XMFLOAT2(0.0f, 0.0f);

		VertexData[1].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
		VertexData[1].Tex = XMFLOAT2(1.0f, 1.0f);

		VertexData[2].Pos = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
		VertexData[2].Tex = XMFLOAT2(0.0f, 1.0f);

		// Second triangle.
		VertexData[3].Pos = XMFLOAT3(left, top, 0.0f);  // Bottom left.
		VertexData[3].Tex = XMFLOAT2(0.0f, 0.0f);

		VertexData[4].Pos = XMFLOAT3(right, top, 0.0f); // Top right.
		VertexData[4].Tex = XMFLOAT2(1.0f, 0.0f);

		VertexData[5].Pos = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
		VertexData[5].Tex = XMFLOAT2(1.0f, 1.0f);

		///////////////////////////Index Buffer ////////////////////////////////

		// Set up the description of the static vertex buffer.
		D3D11_BUFFER_DESC VertexBufferDesc;
		VertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth           = sizeof(Vertex) * m_VertexCount;
		VertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags      = 0;
		VertexBufferDesc.MiscFlags           = 0;
		VertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		D3D11_SUBRESOURCE_DATA VBO;
		VBO.pSysMem          = &VertexData[0];
		VBO.SysMemPitch      = 0;
		VBO.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		hr = pD3D11Device->CreateBuffer(&VertexBufferDesc, &VBO, &m_pVertexBuffer);
		DebugHR(hr);

		/////////////////////////////////Index Buffer ///////////////////////////////////////

		// Set up the description of the static index buffer.
		D3D11_BUFFER_DESC IndexBufferDesc;
		IndexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth           = sizeof(UINT) * m_IndexCount;
		IndexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags      = 0;
		IndexBufferDesc.MiscFlags           = 0;
		IndexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		D3D11_SUBRESOURCE_DATA IBO;
		IBO.pSysMem          = &IndexData[0];
		IBO.SysMemPitch      = 0;
		IBO.SysMemSlicePitch = 0;

		hr = pD3D11Device->CreateBuffer(&IndexBufferDesc, &IBO, &m_pIndexBuffer);
		DebugHR(hr);




	}

	void Plane::InitRTT(ID3D11Device *pD3D11Device, int width, int height)
	{

		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT result;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;


		// Store the width and height of the render texture.
		auto textureWidth = width;
		auto textureHeight = height;

		// Initialize the render target texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = textureWidth;
		textureDesc.Height = textureHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the render target texture.
		result = pD3D11Device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);


		// Setup the description of the render target view.
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create the render target view.
		result = pD3D11Device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);


		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		result = pD3D11Device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);


		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = textureWidth;
		depthBufferDesc.Height = textureHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = pD3D11Device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);


		// Initialize the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = pD3D11Device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);

		//Viewport Infomation
		ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.Width    = static_cast<FLOAT>(m_width);
		m_viewport.Height   = static_cast<FLOAT>(m_height);

	}

}