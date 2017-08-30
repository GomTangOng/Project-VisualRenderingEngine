#include "stdafx.h"
#include "HorizontalInteraceShader.h"
#include "ShaderManager.h"
#include "ElementType.h"
#include "VREngine.h"
#include "OffScreenMesh.h"
#include "SlotType.h"
#include "VREngine.h"
#include "TextureManager.h"

CHorizontalInteraceShader::CHorizontalInteraceShader()
{
}


CHorizontalInteraceShader::~CHorizontalInteraceShader()
{
	CShader::~CShader();
	Memory::Release(m_pOutRTV);
	Memory::Release(m_pOutSRV);
	Memory::Release(m_pOutUAV);
}

void CHorizontalInteraceShader::CreateShader()
{
	SHADER_MANAGER->CreateComputeShaderFromFile(L"Interace.hlsl", "CS_VERTICAL_INTERACE", "cs_5_0", &m_pComputeShader);
}

void CHorizontalInteraceShader::BuildObject()
{
	CreateShader();
	BuildViews(VR_ENGINE->GetWindowWidth(), VR_ENGINE->GetWindowHeight());
}

void CHorizontalInteraceShader::Render()
{
	OnPreRender();

	UINT numGroupsX = (UINT)ceilf(VR_ENGINE->GetWindowWidth() / 16.0f);
	UINT numGroupsY = (UINT)ceilf(VR_ENGINE->GetWindowHeight() / 16.0f);
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
