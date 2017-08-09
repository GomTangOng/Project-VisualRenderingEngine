#include "stdafx.h"
#include "TerrainEntity.h"
#include "ShaderManager.h"
#include "VREngine.h"
#include "BufferStructs.h"
#include "TerrainManager.h"
#include "TerrainMesh.h"

CTerrainEntity::CTerrainEntity()
{
}


CTerrainEntity::~CTerrainEntity()
{
}

void CTerrainEntity::Initalize()
{
	XMMATRIX mtxWorld = XMMatrixIdentity();
	XMStoreFloat4x4(&m_mtxWorld, mtxWorld);
	//XMStoreFloat4x4(&m_mtxView, mtxView);
	//XMStoreFloat4x4(&m_mtxProjection, mtxProj);

	/*ConstantBuffer cb;
	cb.mWorld = m_mtxWorld;
	cb.mView = m_mtxView;
	cb.mProjection = m_mtxProjection;
	*/
	//VS_CB_WORLD_MATRIX cb;
	//cb.mtxWorld = m_mtxWorld;
	//m_boundingBox = NULL;

	SetPositionY(-30, true);
	//SetPositionZ(-0, true);

	//m_pcbWorldMatrix = SHADER_MANAGER->CreateBuffer(sizeof(VS_CB_WORLD_MATRIX), 1, &cb, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	CreateRasterizerState(VR_ENGINE->GetDevice());
}

void CTerrainEntity::Update(const float fTimeElapsed)
{
	CEntity::Update(fTimeElapsed);
}

void CTerrainEntity::OnPrepareRender()
{
	CEntity::OnPrepareRender();
}

void CTerrainEntity::Render()
{
	OnPrepareRender();
	UpdateWorldMatrixConstantBuffer(m_mtxWorld);
	for (auto& mesh : CTerrainManager::GetInstance()->GetMeshes())
	{
		mesh->Render(VR_ENGINE->GetDeviceContext());
	}
}
