#include "stdafx.h"
#include "Tutorial02Shader.h"
#include "ShaderManager.h"
#include "ElementType.h"
#include "VREngine.h"
#include "TriangleMesh.h"


CTutorial02Shader::CTutorial02Shader()
{
}


CTutorial02Shader::~CTutorial02Shader()
{
	
}

void CTutorial02Shader::CreateShader()
{
	LPCSTR vertex_shader_name, pixel_shader_name;

	BuildInputElementDesc(VERTEX_POSITION_ELEMENT);
	GetShaderName(m_iVertexElementType, &vertex_shader_name, &pixel_shader_name, nullptr, nullptr, nullptr);
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

void CTutorial02Shader::BuildObject()
{
	/*CreateShader();

	SimpleVertex vertices[] =
	{
		XMFLOAT3(0.0f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f),
	};
	CTriangleMesh *pMesh = new CTriangleMesh();
	pMesh->CreateMesh(VR_ENGINE->GetDevice(), 3, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vertices->Pos);
		
	m_pMesh.push_back(pMesh);*/
}

void CTutorial02Shader::Render()
{
	OnPreRender();
	CShader::Render();
	
	//VR_ENGINE->GetDeviceContext()->Draw(3, 0);
}

void CTutorial02Shader::OnPreRender()
{
	CShader::OnPreRender();
}
