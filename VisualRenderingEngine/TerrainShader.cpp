#include "stdafx.h"
#include "TerrainShader.h"
#include "ElementType.h"
#include "ShaderManager.h"
#include "TerrainManager.h"
#include "TerrainMesh.h"
#include "VREngine.h"
#include "Entity.h"
CTerrainShader::CTerrainShader()
{
}


CTerrainShader::~CTerrainShader()
{
}

void CTerrainShader::CreateShader()
{
	LPCSTR vertex_shader_name, pixel_shader_name;

	BuildInputElementDesc(VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT);
	GetShaderName(m_iVertexElementType, &vertex_shader_name, nullptr, &pixel_shader_name, nullptr);
	SHADER_MANAGER->CreateVertexShaderFromFile(L"Effect.fx",
		vertex_shader_name,
		"vs_5_0",
		&m_pVertexShader,
		m_pInputElementDesc,
		m_nInputElements,
		&m_pVertexLayout);
	SHADER_MANAGER->CreatePixelShaderFromFile(L"Effect.fx",
		pixel_shader_name,
		"ps_5_0",
		&m_pPixelShader);
}

void CTerrainShader::BuildObject()
{
	CreateShader();
	CTerrainManager::GetInstance()->BuildTerrain(160.0f, 160.0f, 50, 50);
}

void CTerrainShader::Render()
{
	OnPreRender();
	for (auto& e : m_pObjects)
	{
		e->Render();
	}
}

void CTerrainShader::Render(CCamera * pCamera)
{
	OnPreRender();
	for (auto& e : m_pObjects)
	{
		e->Render();
	}
}

void CTerrainShader::OnPreRender()
{
	CShader::OnPreRender();
}
