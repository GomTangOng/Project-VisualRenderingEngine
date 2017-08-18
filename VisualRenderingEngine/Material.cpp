#include "stdafx.h"
#include "Material.h"
#include "VREngine.h"
#include "SlotType.h"
#include "BufferStructs.h"

ID3D11Buffer* CMaterial::m_pcbMaterialColors;

CMaterial::CMaterial()
{
	ZeroMemory(this, sizeof(this));
}


CMaterial::~CMaterial()
{
	Memory::Release(m_pcbMaterialColors);	// TEMP
}

void CMaterial::UpdateConstantBuffer()
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	VR_ENGINE->GetDeviceContext()->Map(m_pcbMaterialColors, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	PS_CB_MATERIAL *pcbMaterial = (PS_CB_MATERIAL *)d3dMappedResource.pData;
	pcbMaterial->ambient  = m_ambient;
	pcbMaterial->diffuse  = m_diffuse;
	pcbMaterial->reflect  = m_reflect;
	pcbMaterial->specular = m_specular;
	VR_ENGINE->GetDeviceContext()->Unmap(m_pcbMaterialColors, 0);
	VR_ENGINE->GetDeviceContext()->PSSetConstantBuffers(PS_CB_SLOT_MATERIAL, 1, &m_pcbMaterialColors);
}
