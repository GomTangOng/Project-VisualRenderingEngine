#include "stdafx.h"
#include "SkyBoxShader.h"
#include "ElementType.h"
#include "ShaderManager.h"
#include "GeometryGenerator.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureManager.h"
#include "VREngine.h"
#include "Entity.h"
#include "Camera.h"
#include "RenderState.h"

CSkyBoxShader::CSkyBoxShader()
{
}


CSkyBoxShader::~CSkyBoxShader()
{
	CShader::~CShader();
}

void CSkyBoxShader::CreateShader()
{
	BuildInputElementDesc(VERTEX_POSITION_ELEMENT);
	SHADER_MANAGER->CreateVertexShaderFromFile(L"SkyBox.hlsl",
		"VS_SKYBOX",
		"vs_5_0",
		&m_pVertexShader,
		m_pInputElementDesc,
		m_nInputElements,
		&m_pVertexLayout);
	SHADER_MANAGER->CreatePixelShaderFromFile(L"SkyBox.hlsl",
		"PS_SKYBOX",
		"ps_5_0",
		&m_pPixelShader);

	//// Depth Stencil & Rasterizer
	//D3D11_DEPTH_STENCIL_DESC dd;
	//ZeroMemory(&dd, sizeof(dd));
	//dd.DepthEnable = false;
	//dd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	//HR(VR_ENGINE->GetDevice()->CreateDepthStencilState(&dd, &m_pDepthStencilState));
}

void CSkyBoxShader::BuildObject()
{
	CreateShader();
}

void CSkyBoxShader::Render()
{
	OnPreRender();
	CShader::Render();
	VR_ENGINE->GetDeviceContext()->OMSetDepthStencilState(NULL, 0);
	VR_ENGINE->GetDeviceContext()->RSSetState(NULL);
}

void CSkyBoxShader::Render(CCamera * pCamera)
{
	OnPreRender();
	for (CEntity *obj : m_pObjects)
	{
		obj->Render();
		m_nVisibleObjects++;
	}
	RENDER_STATE->ClearDepthStencilState();
	RENDER_STATE->ClearRasterState();
}

void CSkyBoxShader::OnPreRender()
{
	CShader::OnPreRender();
	RENDER_STATE->SetDepthStencilState(CRenderState::DEPTH_STENCIL_STATE::DEPTH_STENCIL_SKYBOX, 0);
	RENDER_STATE->SetRasterState(CRenderState::RASTER_STATE::RASTER_CULL_NONE);
}
