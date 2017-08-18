#include "stdafx.h"
#include "LigtingExamShader.h"
#include "ElementType.h"
#include "ShaderManager.h"
#include "VREngine.h"

CLigtingExamShader::CLigtingExamShader()
{
}


CLigtingExamShader::~CLigtingExamShader()
{
}

void CLigtingExamShader::CreateShader()
{
	LPCSTR vertex_shader_name, pixel_shader_name;
	BuildInputElementDesc(VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT);
	GetShaderName(m_iVertexElementType, &vertex_shader_name, &pixel_shader_name, nullptr, nullptr, nullptr);

	//D3D10CreateEffectFromMemory()
	
	//VR_ENGINE->GetDevice()->CreateClassLinkage()
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

void CLigtingExamShader::BuildObject()
{
	CreateShader();
}
