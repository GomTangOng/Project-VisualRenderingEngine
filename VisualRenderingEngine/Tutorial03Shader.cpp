#include "stdafx.h"
#include "Tutorial03Shader.h"
#include "ShaderManager.h"
#include "ElementType.h"
#include "VREngine.h"
#include "Mesh.h"
#include "CubeMesh.h"
#include "CubeEntity.h"
#include "VertexStructs.h"
#include "RenderState.h"
CTutorial03Shader::CTutorial03Shader()
{
}


CTutorial03Shader::~CTutorial03Shader()
{
	CShader::~CShader();
}

void CTutorial03Shader::CreateShader()
{
	LPCSTR vertex_shader_name, pixel_shader_name;

	BuildInputElementDesc(VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_NORMAL_ELEMENT);
	GetShaderName(m_iVertexElementType, &vertex_shader_name, &pixel_shader_name, nullptr, nullptr, nullptr);
	SHADER_MANAGER->CreateVertexShaderFromFile(L"Effect.hlsl",
												vertex_shader_name,
												"vs_5_0",
												&m_pVertexShader,
												m_pInputElementDesc,	
												m_nInputElements,
												&m_pVertexLayout);
	SHADER_MANAGER->CreatePixelShaderFromFile(L"Effect.hlsl",
												pixel_shader_name,
												"ps_5_0",
												&m_pPixelShader);
}

void CTutorial03Shader::BuildObject()
{
	CreateShader();
}

void CTutorial03Shader::Render()
{
	OnPreRender();
	CShader::Render();
}

void CTutorial03Shader::OnPreRender()
{
	CShader::OnPreRender();
}
