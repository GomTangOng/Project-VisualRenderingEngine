#include "stdafx.h"
#include "TSTerrainShader.h"
#include "ShaderManager.h"
#include "ElementType.h"
#include "TerrainManager.h"
#include "TerrainEntity.h"
#include "VREngine.h"

CTSTerrainShader::CTSTerrainShader()
{
}


CTSTerrainShader::~CTSTerrainShader()
{
	CShader::~CShader();
}

void CTSTerrainShader::CreateShader()
{
	LPCSTR vs_name, ps_name, hs_name, ds_name;

	BuildInputElementDesc(VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_TEXTURE_ELEMENT_1);
	GetShaderName(m_iVertexElementType, &vs_name, &ps_name, nullptr, &hs_name, &ds_name);
	
	SHADER_MANAGER->CreateVertexShaderFromFile(L"Terrain.hlsl", vs_name, "vs_5_0", &m_pVertexShader,
												m_pInputElementDesc,
												m_nInputElements,
												&m_pVertexLayout);
	SHADER_MANAGER->CreatePixelShaderFromFile(L"Terrain.hlsl", ps_name, "ps_5_0", &m_pPixelShader);
	SHADER_MANAGER->CreateHullShaderFromFile(L"Terrain.hlsl", hs_name, "hs_5_0", &m_pHullShader);
	SHADER_MANAGER->CreateDomainShaderFromFile(L"Terrain.hlsl", ds_name, "ds_5_0", &m_pDomainShader);
}

void CTSTerrainShader::BuildObject()
{
	CreateShader();
	AddObject(TERRAIN_MANAGER->GetTerrainEntity());
}

void CTSTerrainShader::Render()
{
	CShader::Render();
	VR_ENGINE->GetDeviceContext()->HSSetShader(0, nullptr, 0);
	VR_ENGINE->GetDeviceContext()->DSSetShader(0, nullptr, 0);
}

void CTSTerrainShader::Render(CCamera * pCamera)
{
	static_cast<CTerrainEntity *>(m_pObjects[0])->SetCamera(pCamera);
	CShader::Render();
	VR_ENGINE->GetDeviceContext()->HSSetShader(0, nullptr, 0);
	VR_ENGINE->GetDeviceContext()->DSSetShader(0, nullptr, 0);
}

void CTSTerrainShader::OnPreRender()
{
	CShader::OnPreRender();
}
