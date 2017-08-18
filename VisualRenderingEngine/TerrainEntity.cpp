#include "stdafx.h"
#include "TerrainEntity.h"
#include "ShaderManager.h"
#include "VREngine.h"
#include "BufferStructs.h"
#include "TerrainManager.h"
#include "TerrainMesh.h"
#include "SlotType.h"
#include "Camera.h"
#include "Texture.h"

CTerrainEntity::CTerrainEntity()
{
}


CTerrainEntity::~CTerrainEntity()
{
	Memory::Delete(m_pMaterial);
}

void CTerrainEntity::UpdateTesslationFactor()
{
	XMStoreFloat4x4(&m_mtxViewProj, m_pCamera->GetViewProjectionMatrixXM());	
	XMFLOAT4 view_plane[6];
	XMMATRIX mtxViewProj = XMLoadFloat4x4(&m_pCamera->GetProjectionMatrix());
	Math::ExtractFrustumPlanes(view_plane, mtxViewProj);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	VR_ENGINE->GetDeviceContext()->Map(m_pcbTessBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	TS_CB_FACTOR *pcbTessFactors = (TS_CB_FACTOR *)d3dMappedResource.pData;
	pcbTessFactors->viewProjection = m_mtxViewProj;
	pcbTessFactors->camPosW = *m_pCamera->GetPosition();
	pcbTessFactors->minDist = m_fMinDist;
	pcbTessFactors->maxDist = m_fMaxDist;
	pcbTessFactors->minTess = m_fMinTess;
	pcbTessFactors->maxTess = m_fMaxTess;
	pcbTessFactors->texelCellSpaceU = m_fTexelCellSpaceU;
	pcbTessFactors->texelCellSpaceV = m_fTexelCellSpaceV;
	pcbTessFactors->worldCellSpace  = m_nCellSpacing;	
	memcpy(pcbTessFactors->worldFrustumPlanes, view_plane, sizeof(XMFLOAT4) * 6);
	VR_ENGINE->GetDeviceContext()->Unmap(m_pcbTessBuffer, 0);
	VR_ENGINE->GetDeviceContext()->HSSetConstantBuffers(HS_CB_SLOT_TESSLATION, 1, &m_pcbTessBuffer);
}

void CTerrainEntity::Initalize()
{
	XMMATRIX mtxWorld = XMMatrixIdentity();
	XMStoreFloat4x4(&m_mtxWorld, mtxWorld);
	
	//SetPositionY(-30, true);
	//
	//CEntity::Update(0.0f);
	//CreateRasterizerState(VR_ENGINE->GetDevice());
	m_szHeightMapFileName = L"../Assets/HeightMaps/terrain.raw";
	m_nWidth       = 2049.0f;
	m_nHeight      = 2049.0f;
	m_nHeightScale = 50.0f;
	m_nCellSpacing = 0.5f;
	m_fMinDist     = 20.0f;
	m_fMaxDist	   = 500.0f;
	m_fMinTess     = 0.0f;
	m_fMaxTess     = 6.0f;
	m_fTexelCellSpaceU = 1.0f / m_nWidth;
	m_fTexelCellSpaceV = 1.0f / m_nHeight;
	m_pcbTessBuffer = SHADER_MANAGER->CreateBuffer(sizeof(TS_CB_FACTOR), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	// To do : 나중에 MaterialManager 만들것...
	m_pMaterial = new CMaterial();
	m_pMaterial->SetAmbient(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pMaterial->SetDiffuse(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pMaterial->SetSpecular(XMFLOAT4(0.0f, 0.0f, 0.0f, 64.0f));
	m_pMaterial->SetReflect(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

void CTerrainEntity::Update(const float fTimeElapsed)
{
	//CEntity::Update(fTimeElapsed);
}

// 변수들 셋 해줄꺼 찾아서 셋 해야함...

void CTerrainEntity::OnPrepareRender()
{
	//CEntity::OnPrepareRender();
	UpdateWorldMatrixConstantBuffer(m_mtxWorld);
	UpdateTesslationFactor();
	m_pMaterial->UpdateConstantBuffer();

	TERRAIN_MANAGER->GetHeightMap()->VSSetTextureAndSampler();
	TERRAIN_MANAGER->GetHeightMap()->PSSetTextureAndSampler();
	TERRAIN_MANAGER->GetHeightMap()->HSSetTextureAndSampler();
	TERRAIN_MANAGER->GetHeightMap()->DSSetTextureAndSampler();
	m_pTexture->PSSetTextureAndSampler();	
}

void CTerrainEntity::Render()
{
	OnPrepareRender();
	for (CMesh* pMesh : m_pMeshes)
	{
		pMesh->Render(VR_ENGINE->GetDeviceContext());
	}
}

// 하나의 텍스쳐 또는 상수버퍼를 여러개의 쉐이더에 동시에 사용 가능????