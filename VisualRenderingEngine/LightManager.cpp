#include "stdafx.h"
#include "LightManager.h"
#include "Light.h"
#include "BufferStructs.h"
#include "VREngine.h"
#include "SlotType.h"
#include "ShaderManager.h"

ID3D11Buffer *CLightManager::m_pLightConstantBuffer;

CLightManager::CLightManager()
{
	
}


CLightManager::~CLightManager()
{
	Memory::Release(m_pLightConstantBuffer);
	for (int i = 0; i < MAX_LIGHT; ++i)
	{
		Memory::Delete(m_ppDirectionLights[i]);
		Memory::Delete(m_ppPointLights[i]);
		Memory::Delete(m_ppSpotLights[i]);
	}
	
}

bool CLightManager::Initalize(ID3D11Device * pDevice)
{
	m_pDevice = pDevice;
	m_pLightConstantBuffer = SHADER_MANAGER->CreateBuffer(sizeof(PS_CB_LIGHT), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	for (int i = 0; i < MAX_LIGHT; ++i)
	{
		m_ppDirectionLights[i] = new CDirectionLight();
		m_ppPointLights[i]     = new CPointLight();
		m_ppSpotLights[i]      = new CSpotLight();
	}
	return true;
}

void CLightManager::ShotDown()
{
	Memory::DeleteArray(m_ppDirectionLights);
	Memory::DeleteArray(m_ppPointLights);
	Memory::DeleteArray(m_ppSpotLights);
}

void CLightManager::ClearLight()
{
	ZeroMemory(&m_ppDirectionLights, sizeof(CDirectionLight) * MAX_LIGHT); m_nDirLight = 0;
	ZeroMemory(&m_ppPointLights, sizeof(CPointLight) * MAX_LIGHT); m_nPointLight = 0;
	ZeroMemory(&m_ppSpotLights, sizeof(CSpotLight) * MAX_LIGHT); m_nSpotLight = 0;
}

void CLightManager::AddDirectionLight(CDirectionLight * pDirLight)
{
	if (MAX_LIGHT <= m_nDirLight) return;

	*m_ppDirectionLights[m_nDirLight] = *pDirLight;
	m_ppDirectionLights[m_nDirLight++]->m_pad = 1.0f;
}

void CLightManager::AddPointLight(CPointLight * pPointLight)
{
	if (MAX_LIGHT <= m_nPointLight) return;

	*m_ppPointLights[m_nPointLight] = *pPointLight;
	m_ppPointLights[m_nPointLight++]->m_pad = 1.0f;
}

void CLightManager::AddSpotLight(CSpotLight * pSpotLight)
{
	if (MAX_LIGHT <= m_nSpotLight) return;

	*m_ppSpotLights[m_nSpotLight] = *pSpotLight;
	m_ppSpotLights[m_nSpotLight++]->m_pad = 1.0f;
}

void CLightManager::UpdateLightConstantBuffer(XMFLOAT3 * pCamPos)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	HR(VR_ENGINE->GetDeviceContext()->Map(m_pLightConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource));
	PS_CB_LIGHT *pcbLight = (PS_CB_LIGHT *)d3dMappedResource.pData;

	for (int i = 0; i < m_nDirLight; ++i) pcbLight->dirLight[i]   = *m_ppDirectionLights[i];
	for (int i = 0; i < m_nPointLight; ++i) pcbLight->pointLight[i] = *m_ppPointLights[i];
	for (int i = 0; i < m_nSpotLight; ++i) pcbLight->spotLight[i] = *m_ppSpotLights[i];
	
	pcbLight->camPos = *pCamPos;

	VR_ENGINE->GetDeviceContext()->Unmap(m_pLightConstantBuffer, 0);
	VR_ENGINE->GetDeviceContext()->PSSetConstantBuffers(PS_CB_SLOT_LIGHT, 1, &m_pLightConstantBuffer);
}