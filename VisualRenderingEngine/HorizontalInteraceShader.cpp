#include "stdafx.h"
#include "HorizontalInteraceShader.h"
#include "ShaderManager.h"
#include "ElementType.h"
#include "VREngine.h"
#include "OffScreenMesh.h"
#include "SlotType.h"
CHorizontalInteraceShader::CHorizontalInteraceShader()
{
}


CHorizontalInteraceShader::~CHorizontalInteraceShader()
{
	Memory::Delete(m_pRenderMesh);
	CShader::~CShader();
}

void CHorizontalInteraceShader::CreateShader()
{
	BuildInputElementDesc(VertexElementType::VERTEX_POSITION_ELEMENT | VertexElementType::VERTEX_TEXTURE_ELEMENT_0);
	
	SHADER_MANAGER->CreateVertexShaderFromFile(L"Effect.hlsl", "VS_TEXTURED", "vs_5_0", &m_pVertexShader, m_pInputElementDesc, m_nInputElements, &m_pVertexLayout);
	SHADER_MANAGER->CreatePixelShaderFromFile(L"Effect.hlsl", "PS_TEXTURED", "ps_5_0", &m_pPixelShader);
	//SHADER_MANAGER->CreateVertexShaderFromFile(L"Effect.fx", "VS_EXAMPLE", "vs_5_0", &m_pVertexShader, &)
	//SHADER_MANAGER->CreateComputeShaderFromFile(L"Interace.hlsl", "CS_HORIZONTAL_INTERACE", "cs_5_0", &m_pComputeShader);
}

void CHorizontalInteraceShader::BuildObject()
{
	CreateShader();
	m_pRenderMesh = new COffScreenMesh(VR_ENGINE->GetWindowWidth(), VR_ENGINE->GetWindowHeight());
}

void CHorizontalInteraceShader::Render()
{
	OnPreRender();
	m_pRenderMesh->Render(VR_ENGINE->GetDeviceContext());

	ID3D11ShaderResourceView *pSRVNull[1]{ nullptr };
	VR_ENGINE->GetDeviceContext()->PSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, pSRVNull);
}

void CHorizontalInteraceShader::OnPreRender()
{
	CShader::OnPreRender();
	
	ID3D11ShaderResourceView *pSRVNull[1]{ nullptr };
	//VR_ENGINE->GetDeviceContext()->PSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, pSRVNull);
	VR_ENGINE->GetDeviceContext()->PSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, VR_ENGINE->GetOffScreenSRV());
}
