#include "stdafx.h"
#include "HorizontalInteraceShader.h"
#include "ShaderManager.h"

CHorizontalInteraceShader::CHorizontalInteraceShader()
{
}


CHorizontalInteraceShader::~CHorizontalInteraceShader()
{
	CShader::~CShader();
}

void CHorizontalInteraceShader::CreateShader()
{
	SHADER_MANAGER->CreateComputeShaderFromFile(L"Interace.hlsl", "CS_HORIZONTAL_INTERACE", "cs_5_0", &m_pComputeShader);
}

void CHorizontalInteraceShader::BuildObject()
{
	CreateShader();
}

void CHorizontalInteraceShader::Render()
{
}

void CHorizontalInteraceShader::OnPreRender()
{
}
