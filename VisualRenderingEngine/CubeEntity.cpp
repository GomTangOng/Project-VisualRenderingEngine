#include "stdafx.h"
#include "CubeEntity.h"
#include "Entity.h"
#include "ShaderManager.h"
#include "VREngine.h"
#include "BufferStructs.h"
#include "TextureManager.h"
CCubeEntity::CCubeEntity()
{
}


CCubeEntity::~CCubeEntity()
{
}

void CCubeEntity::CreateRasterizerState(ID3D11Device * pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//	d3dRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pRasterizerState);
}

void CCubeEntity::Initalize()
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

	//if(!m_pcbWorldMatrix) m_pcbWorldMatrix = SHADER_MANAGER->CreateBuffer(sizeof(VS_CB_WORLD_MATRIX), 1, &cb, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	SetTexture(TEXTURE_MANAGER->GetTexture(L"Wood01"));
	CreateRasterizerState(VR_ENGINE->GetDevice());
	InitBoudingBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1, 1, 1));
}

void CCubeEntity::Update(const float fTimeElapsed)
{
	CEntity::Update(fTimeElapsed);
}

void CCubeEntity::OnPrepareRender()
{
	CEntity::OnPrepareRender();
}

void CCubeEntity::Render()
{
	CEntity::Render();
}
