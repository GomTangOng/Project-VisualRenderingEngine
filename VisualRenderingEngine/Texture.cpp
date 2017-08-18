#include "stdafx.h"
#include "Texture.h"
#include "VREngine.h"
#include "SlotType.h"

CTexture::CTexture()
{
}


CTexture::~CTexture()
{
}

void CTexture::PSSetTextureAndSampler()
{
	if (m_pSRV)          VR_ENGINE->GetDeviceContext()->PSSetShaderResources(m_nTextureSlot, 1, &m_pSRV);
	if (m_pSamplerState) VR_ENGINE->GetDeviceContext()->PSSetSamplers(m_nSamplerSlot, 1, &m_pSamplerState);
}

void CTexture::VSSetTextureAndSampler()
{
	if (m_pSRV)          VR_ENGINE->GetDeviceContext()->VSSetShaderResources(m_nTextureSlot, 1, &m_pSRV);
	if (m_pSamplerState) VR_ENGINE->GetDeviceContext()->VSSetSamplers(m_nSamplerSlot, 1, &m_pSamplerState);
}

void CTexture::HSSetTextureAndSampler()
{
	if (m_pSRV)          VR_ENGINE->GetDeviceContext()->HSSetShaderResources(m_nTextureSlot, 1, &m_pSRV);
	if (m_pSamplerState) VR_ENGINE->GetDeviceContext()->HSSetSamplers(m_nSamplerSlot, 1, &m_pSamplerState);
}

void CTexture::DSSetTextureAndSampler()
{
	if (m_pSRV)          VR_ENGINE->GetDeviceContext()->DSSetShaderResources(m_nTextureSlot, 1, &m_pSRV);
	if (m_pSamplerState) VR_ENGINE->GetDeviceContext()->DSSetSamplers(m_nSamplerSlot, 1, &m_pSamplerState);
}
