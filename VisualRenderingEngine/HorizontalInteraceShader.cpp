#include "stdafx.h"
#include "HorizontalInteraceShader.h"
#include "ShaderManager.h"
#include "ElementType.h"
#include "VREngine.h"
#include "OffScreenMesh.h"
#include "SlotType.h"
#include "VREngine.h"
#include "TextureManager.h"
#include "BufferStructs.h"

CHorizontalInteraceShader::CHorizontalInteraceShader()
{
}


CHorizontalInteraceShader::~CHorizontalInteraceShader()
{
	CShader::~CShader();
	Memory::Release(m_pOutRTV);
	Memory::Release(m_pOutSRV);
	Memory::Release(m_pOutUAV);
	Memory::Release(m_pcbWindowSizeBuffer);
}

void CHorizontalInteraceShader::CreateShader()
{
	SHADER_MANAGER->CreateComputeShaderFromFile(L"Interace.hlsl", "CS_HORIZONTAL_INTERACE", "cs_5_0", &m_pComputeShader);
}

void CHorizontalInteraceShader::BuildObject()
{
	CreateShader();
	BuildViews(VR_ENGINE->GetWindowWidth(), VR_ENGINE->GetWindowHeight());
	m_pcbWindowSizeBuffer = SHADER_MANAGER->CreateBuffer(sizeof(CS_CB_WINDOWSIZE), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
}

void CHorizontalInteraceShader::Render()
{
	OnPreRender();

	UINT numGroupsX = (UINT)ceilf(VR_ENGINE->GetWindowWidth() / 32.0f);
	UINT numGroupsY = (UINT)ceilf(VR_ENGINE->GetWindowHeight() / 32.0f);
	VR_ENGINE->GetDeviceContext()->Dispatch(numGroupsX, numGroupsY, 1);

	ID3D11ShaderResourceView *pSRVNull[1]{ nullptr };
	ID3D11UnorderedAccessView* pUAVNull[1]{ nullptr };
	VR_ENGINE->GetDeviceContext()->CSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, pSRVNull);
	VR_ENGINE->GetDeviceContext()->CSSetUnorderedAccessViews(UnOrderedAccessView::UAV_RENDER_TEXTURE, 1, pUAVNull, 0);
	//VR_ENGINE->GetDeviceContext()->CSSetUnorderedAccessViews(UnOrderedAccessView::UAV_RENDER_TEXTURE, 1, pUAVNull, 0);
	VR_ENGINE->GetDeviceContext()->CSSetShader(nullptr, nullptr, 0);
}

void CHorizontalInteraceShader::OnPreRender()
{
	CShader::OnPreRender();
	ID3D11ShaderResourceView *pSRVNull[1]{ nullptr };
	ID3D11RenderTargetView* pRTVNull[1]{ nullptr };
	VR_ENGINE->GetDeviceContext()->OMSetRenderTargets(1, pRTVNull, VR_ENGINE->GetDSV());
	VR_ENGINE->GetDeviceContext()->CSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, VR_ENGINE->GetOffScreenSRV());
	auto sam = TEXTURE_MANAGER->GetSampler(L"BaseSampler");
	VR_ENGINE->GetDeviceContext()->CSSetSamplers(SamplerSlot::SAMPLER_BASIC, 1, &sam);
	//VR_ENGINE->GetDeviceContext()->CSSetUnorderedAccessViews(UnOrderedAccessView::UAV_RENDER_TEXTURE, 1, VR_ENGINE->GetUAV(), nullptr);
	VR_ENGINE->GetDeviceContext()->CSSetUnorderedAccessViews(UnOrderedAccessView::UAV_RENDER_TEXTURE, 1, &m_pOutUAV, nullptr);
	UpdateConstantBuffer();	// Update Window Size
}

void CHorizontalInteraceShader::BuildViews(const UINT width, const UINT height)
{
//	Memory::Release(m_pOutRTV);
//	Memory::Release(m_pOutUAV);

	ID3D11Texture2D* pOut;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
	td.Width     = width;
	td.Height    = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format    = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count   = 1;
	td.SampleDesc.Quality = 0;
	td.Usage              = D3D11_USAGE_DEFAULT;
	td.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	td.CPUAccessFlags     = 0;
	td.MiscFlags          = 0;

	HR(VR_ENGINE->GetDevice()->CreateTexture2D(&td, nullptr, &pOut));
	HR(VR_ENGINE->GetDevice()->CreateShaderResourceView(pOut, nullptr, &m_pOutSRV));
	//VR_ENGINE->GetDevice()->CreateRenderTargetView(pOut, nullptr, &m_pOutRTV);
	HR(VR_ENGINE->GetDevice()->CreateUnorderedAccessView(pOut, nullptr, &m_pOutUAV));
	Memory::Release(pOut);
}

void CHorizontalInteraceShader::UpdateConstantBuffer()
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	VR_ENGINE->GetDeviceContext()->Map(m_pcbWindowSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	CS_CB_WINDOWSIZE *pcbWinSize = (CS_CB_WINDOWSIZE *)d3dMappedResource.pData;
	pcbWinSize->fWidth  = VR_ENGINE->GetWindowWidth();
	pcbWinSize->fHeight = VR_ENGINE->GetWindowHeight();
	VR_ENGINE->GetDeviceContext()->Unmap(m_pcbWindowSizeBuffer, 0);
	VR_ENGINE->GetDeviceContext()->CSSetConstantBuffers(4, 1, &m_pcbWindowSizeBuffer);
}
