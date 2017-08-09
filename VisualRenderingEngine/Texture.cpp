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
	VR_ENGINE->GetDeviceContext()->PSSetShaderResources(TextureSlot::TEXTURE_BOX01, 1, &m_pSRV);
	VR_ENGINE->GetDeviceContext()->PSSetSamplers(SamplerSlot::SAMPLER_BASIC, 1, &m_pSamplerState);
}
