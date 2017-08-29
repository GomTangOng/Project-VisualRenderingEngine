#include "stdafx.h"
#include "OffScreenRenderShader.h"
#include "ShaderManager.h"
#include "ElementType.h"
#include "VREngine.h"
#include "OffScreenMesh.h"
#include "SlotType.h"

COffScreenRenderShader::COffScreenRenderShader()
{
}


COffScreenRenderShader::~COffScreenRenderShader()
{
	Memory::Delete(m_pRenderMesh);
	CShader::~CShader();
}

void COffScreenRenderShader::CreateShader()
{
	BuildInputElementDesc(VertexElementType::VERTEX_POSITION_ELEMENT | VertexElementType::VERTEX_TEXTURE_ELEMENT_0);

	SHADER_MANAGER->CreateVertexShaderFromFile(L"Effect.hlsl", "VS_TEXTURED", "vs_5_0", &m_pVertexShader, m_pInputElementDesc, m_nInputElements, &m_pVertexLayout);
	SHADER_MANAGER->CreatePixelShaderFromFile(L"Effect.hlsl", "PS_TEXTURED", "ps_5_0", &m_pPixelShader);
}

void COffScreenRenderShader::BuildObject()
{
	CreateShader();
	m_pRenderMesh = new COffScreenMesh(VR_ENGINE->GetWindowWidth(), VR_ENGINE->GetWindowHeight());
}

void COffScreenRenderShader::Render()
{
	OnPreRender();
	m_pRenderMesh->Render(VR_ENGINE->GetDeviceContext());

	ID3D11ShaderResourceView *pSRVNull[1]{ nullptr };
	VR_ENGINE->GetDeviceContext()->PSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, pSRVNull);
}

void COffScreenRenderShader::OnPreRender()
{
	CShader::OnPreRender();

	//ID3D11ShaderResourceView *pSRVNull[1]{ nullptr };
	//VR_ENGINE->GetDeviceContext()->PSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, pSRVNull);
	
	//VR_ENGINE->GetDeviceContext()->PSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, VR_ENGINE->GetOffScreenSRV());
}
