#include "stdafx.h"
#include "TextureManager.h"
#include "Utility.h"
#include "Texture.h"
#include <DDSTextureLoader.h>
#include "ShaderManager.h"
#include "BufferStructs.h"
#include "Material.h"
#include "SlotType.h"
CTextureManager::CTextureManager()
{
}


CTextureManager::~CTextureManager()
{
	for (auto& t : m_mapTextures)
	{
		Memory::Release(t.second->GetSRV());
		Memory::Delete(t.second);
	}
}

bool CTextureManager::Initalize(ID3D11Device * pDevice)
{
	m_pDevice = pDevice;
	CMaterial::m_pcbMaterialColors = SHADER_MANAGER->CreateBuffer(sizeof(PS_CB_MATERIAL), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	// Create Sampler State
	ID3D11SamplerState *pBaseSampler;
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.MinLOD = 0;
	sd.MaxLOD = 0;
	HR(m_pDevice->CreateSamplerState(&sd, &pBaseSampler));
	AddSampler(L"BaseSampler", pBaseSampler);

	ID3D11SamplerState *pLinearSampler;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	HR(m_pDevice->CreateSamplerState(&sd, &pLinearSampler));
	AddSampler(L"LinearSampler", pLinearSampler);


	ID3D11SamplerState *pHeightmapSampler;
	sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	HR(m_pDevice->CreateSamplerState(&sd, &pHeightmapSampler));
	AddSampler(L"HeightmapSampler", pHeightmapSampler);

	// Load Textures 
	CTexture *pWood01 = LoadTextureFromFile(L"../Assets/Textures/WoodCrate01.dds");
	pWood01->SetTextureSlot(TextureSlot::TEXTURE_BOX01);
	pWood01->SetSamplerSlot(SamplerSlot::SAMPLER_BASIC);
	pWood01->SetSampler(pBaseSampler);

	AddTexture(L"Wood01", pWood01);

	CTexture *pGrass = LoadTextureFromFile(L"../Assets/Textures/grass.dds");
	pGrass->SetTextureSlot(TextureSlot::TEXTURE_GRASS);
	pGrass->SetSamplerSlot(SamplerSlot::SAMPLER_LINEAR);
	pGrass->SetSampler(pLinearSampler);

	AddTexture(L"Grass", pGrass);

	CTexture *pSkyBox = LoadTextureFromFile(L"../Assets/Textures/grasscube1024.dds");
	pSkyBox->SetTextureSlot(TextureSlot::TEXTURE_GRASS_SKYBOX);
	pSkyBox->SetSamplerSlot(SamplerSlot::SAMPLER_BASIC);
	pSkyBox->SetSampler(pLinearSampler);

	AddTexture(L"Grass_SkyBox", pSkyBox);

	//CTexture *pHeightMap = LoadTextureFromFile(L"../Assets/HeightMaps/terrain.raw");
	//AddTexture(L"HeightMap", pHeightMap);

	return true;
}

void CTextureManager::ShutDown()
{
	for (auto& tex : m_mapTextures) Memory::Delete(tex.second);
	for (auto& sam : m_mapSamplers) Memory::Release(sam.second);
}


CTexture * CTextureManager::LoadTextureFromFile(wstring file_name, ID3D11Resource *pTex2D)
{
	CTexture *pTex = new CTexture();
	ID3D11ShaderResourceView *pSRV;

	HR(CreateDDSTextureFromFile(m_pDevice, file_name.c_str(), &pTex2D, &pSRV));
	
	pTex->SetSRV(pSRV);

	return pTex;
}

void CTextureManager::AddTexture(wstring tex_name, CTexture * pTex)
{
	m_mapTextures[tex_name] = pTex;
}

void CTextureManager::RemoveTexture(wstring tex_name)
{
	m_mapTextures.erase(tex_name);
}

void CTextureManager::AddSampler(wstring sam_name, ID3D11SamplerState * pSampler)
{
	m_mapSamplers[sam_name] = pSampler;
}

void CTextureManager::RemoveSampler(wstring sam_name)
{
	m_mapSamplers.erase(sam_name);
}

CTexture * CTextureManager::GetTexture(wstring tex_name)
{
	return m_mapTextures[tex_name];
}
