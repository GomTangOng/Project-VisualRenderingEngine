#include "stdafx.h"
#include "Entity.h"
#include "SlotType.h"
#include "VREngine.h"
#include "Mesh.h"
#include "Utility.h"
#include "ShaderManager.h"
#include "BufferStructs.h"
#include "Camera.h"
#include "Material.h"
#include "Texture.h"

ID3D11Buffer* CEntity::m_pcbWorldMatrix;
//ID3D11Buffer* CEntity::m_pcbMaterialColors;
//ID3D11Buffer* CEntity::m_pcbWorldViewProjMatrix;
ID3D11Buffer* CEntity::m_pcbViewProjMatrix;

CEntity::CEntity()
{
	m_fPitch = m_fRoll = m_fYaw = 0.0f;
	XMStoreFloat4x4(&m_mtxLocal, XMMatrixIdentity());
	XMStoreFloat4x4(&m_mtxWorld, XMMatrixIdentity());
	//XMStoreFloat4x4(&m_mtxView, XMMatrixIdentity());
	//XMStoreFloat4x4(&m_mtxProjection, XMMatrixIdentity());

	XMStoreFloat3(&m_vRight, XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
	XMStoreFloat3(&m_vUp, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	XMStoreFloat3(&m_vLook, XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	XMStoreFloat3(&m_vPosition, XMVectorZero());
	m_pChild = m_pParent = nullptr;
	InitBoudingBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
}


CEntity::~CEntity()
{
	Memory::DeleteVector(m_pMeshes);	// TEMP
	//for_each(m_pMeshes.begin(), m_pMeshes.end(), &Memory::Delete<CMesh *>);
}

void CEntity::Translate(const float x, const float y, const float z, const bool local)
{
	XMVECTOR posXM = XMLoadFloat3(&m_vPosition);
	XMVECTOR shift = XMVectorSet(x, y, z, 0);
	posXM = XMVectorAdd(posXM, shift);

	if (local)
	{
		XMStoreFloat3(&m_vPosition, XMVector3TransformNormal(posXM, XMLoadFloat4x4(&m_mtxLocal)));
	}
	else
	{
		m_vPosition.x += x;
		m_vPosition.y += y;
		m_vPosition.z += z;
	}
}

void CEntity::Rotate(const float x, const float y, const float z)
{
	XMMATRIX mtx;

	m_fPitch += x;
	mtx = XMMatrixRotationAxis(XMLoadFloat3(&m_vRight), XMConvertToRadians(x));
	XMStoreFloat3(&m_vLook, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), mtx));
	XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&m_vUp), mtx));
	
	m_fYaw += y;
	mtx = XMMatrixRotationAxis(XMLoadFloat3(&m_vUp), XMConvertToRadians(y));
	XMStoreFloat3(&m_vLook, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), mtx));
	XMStoreFloat3(&m_vRight, XMVector3TransformNormal(XMLoadFloat3(&m_vRight), mtx));

	m_fRoll += z;
	mtx = XMMatrixRotationAxis(XMLoadFloat3(&m_vLook), XMConvertToRadians(z));
	XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&m_vUp), mtx));
	XMStoreFloat3(&m_vRight, XMVector3TransformNormal(XMLoadFloat3(&m_vRight), mtx));
	
	Orthogonalize();
}

void CEntity::RotationX(const float x)
{
	XMMATRIX mtx;

	m_fPitch += x;
	mtx = XMMatrixRotationAxis(XMLoadFloat3(&m_vRight), XMConvertToRadians(x));
	XMStoreFloat3(&m_vLook, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), mtx));
	XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&m_vUp), mtx));
	
	Orthogonalize();
}

void CEntity::RotationY(const float y)
{
	XMMATRIX mtx;
	
	//XMMATRIX out = XMMatrixRotationY(XMConvertToRadians(y));
	//XMFLOAT3 temp = XMFLOAT3(1.0f, 0.0f, 0.0f);
	//XMStoreFloat3(&temp, XMVector3TransformNormal(XMLoadFloat3(&temp), out));

	m_fYaw += y;
	mtx = XMMatrixRotationAxis(XMLoadFloat3(&m_vUp), XMConvertToRadians(y));
	XMStoreFloat3(&m_vLook, XMVector3TransformNormal(XMLoadFloat3(&m_vLook), mtx));
	XMStoreFloat3(&m_vRight, XMVector3TransformNormal(XMLoadFloat3(&m_vRight), mtx));
	
	Orthogonalize();
}

void CEntity::RotationZ(const float z)
{
	XMMATRIX mtx;
	
	m_fRoll += z;
	mtx = XMMatrixRotationAxis(XMLoadFloat3(&m_vLook), XMConvertToRadians(z));
	XMStoreFloat3(&m_vUp, XMVector3TransformNormal(XMLoadFloat3(&m_vUp), mtx));
	XMStoreFloat3(&m_vRight, XMVector3TransformNormal(XMLoadFloat3(&m_vRight), mtx));

	Orthogonalize();
}

void CEntity::Orthogonalize()
{
	XMStoreFloat3(&m_vLook, XMVector3Normalize(XMLoadFloat3(&m_vLook)));
	XMStoreFloat3(&m_vRight, XMVector3Cross(XMLoadFloat3(&m_vUp), XMLoadFloat3(&m_vLook)));
	XMStoreFloat3(&m_vRight, XMVector3Normalize(XMLoadFloat3(&m_vRight)));
	XMStoreFloat3(&m_vUp, XMVector3Cross(XMLoadFloat3(&m_vLook), XMLoadFloat3(&m_vRight)));
	XMStoreFloat3(&m_vUp, XMVector3Normalize(XMLoadFloat3(&m_vUp)));
	//XMFLOAT4 temp;
	//XMStoreFloat4(&temp, XMVector3Cross(XMLoadFloat3(&m_vLook), XMLoadFloat3(&m_vRight)));
}

void CEntity::Update(XMFLOAT4X4 * pMtxParent)
{
	m_mtxWorld = m_mtxLocal;
	
	if (m_pParent) XMStoreFloat4x4(&m_mtxWorld, XMMatrixMultiply(XMLoadFloat4x4(&m_mtxLocal), XMLoadFloat4x4(pMtxParent)));
	if (m_pChild) m_pChild->Update(pMtxParent);

	UpdateBoundingBox();
}

void CEntity::UpdateBoundingBox()
{
	BoundingBox box;
	box.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	box.Extents = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_boundingBox = box;
	m_boundingBox.Transform(m_boundingBox, XMLoadFloat4x4(&m_mtxWorld));	// 여기 고쳐야한다...
}

void CEntity::CreateRasterizerState(ID3D11Device * pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//	d3dRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pRasterizerState);
}

void CEntity::InitBoudingBox(XMFLOAT3 & centerPos, XMFLOAT3 & extents)
{
	m_boundingBox.Center = centerPos;
	m_boundingBox.Extents = extents;
}

void CEntity::InitConstantBuffer()
{
	//int n = sizeof(PS_CB_MATERIAL);
	m_pcbWorldMatrix    = SHADER_MANAGER->CreateBuffer(sizeof(VS_CB_WORLD_MATRIX), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	m_pcbViewProjMatrix = SHADER_MANAGER->CreateBuffer(sizeof(VS_CB_VIEW_PROJECTION_MATRIX), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	//m_pcbMaterialColors = SHADER_MANAGER->CreateBuffer(sizeof(PS_CB_MATERIAL), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
}

void CEntity::ReleaseConctantBuffer()
{
	Memory::Release(m_pcbWorldMatrix);
	Memory::Release(m_pcbViewProjMatrix);
	//Memory::Release(m_pcbMaterialColors);
}

void CEntity::UpdateWorldMatrixConstantBuffer(const XMFLOAT4X4 & pmtxWorld)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	VR_ENGINE->GetDeviceContext()->Map(m_pcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	XMMATRIX outMtx = XMMatrixTranspose(XMLoadFloat4x4(&pmtxWorld));
	XMStoreFloat4x4(&pcbWorldMatrix->mtxWorld, outMtx);
	VR_ENGINE->GetDeviceContext()->Unmap(m_pcbWorldMatrix, 0);
	VR_ENGINE->GetDeviceContext()->VSSetConstantBuffers(VS_CB_SLOT_WORLD_MATRIX, 1, &m_pcbWorldMatrix);
}

void CEntity::UpdateViewProjectionConstantBuffer(XMFLOAT4X4 & pmtxCamera, XMFLOAT4X4 & pmtxProjection)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	VR_ENGINE->GetDeviceContext()->Map(m_pcbViewProjMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_VIEW_PROJECTION_MATRIX *pcbViewProj = (VS_CB_VIEW_PROJECTION_MATRIX *)d3dMappedResource.pData;
	XMMATRIX outView = XMMatrixTranspose(XMLoadFloat4x4(&pmtxCamera));
	XMMATRIX outProj = XMMatrixTranspose(XMLoadFloat4x4(&pmtxProjection));
	XMStoreFloat4x4(&pcbViewProj->mtxView, outView);
	XMStoreFloat4x4(&pcbViewProj->mtxProjection, outProj);
	VR_ENGINE->GetDeviceContext()->Unmap(m_pcbViewProjMatrix, 0);
	VR_ENGINE->GetDeviceContext()->VSSetConstantBuffers(VS_CB_SLOT_CAMERA_PROJECTION_MATRIX, 1, &m_pcbViewProjMatrix);
}


void CEntity::UpdateVertexShaderConstantBuffer(XMFLOAT4X4 & pmtxWorld, XMFLOAT4X4 & pmtxCamera, XMFLOAT4X4 & pmtxProjection)
{
	//HRESULT hr;
	//
	//D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	//if (FAILED(hr = VR_ENGINE->GetDeviceContext()->Map(m_pcbWorldViewProjMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource)))
	//{
	//	cout << endl;
	//}
	//ConstantBuffer *pcbWorldViewProMatrix = (ConstantBuffer *)d3dMappedResource.pData;
	//XMMATRIX outMtxWorld = XMMatrixTranspose(XMLoadFloat4x4(&pmtxWorld));
	//XMMATRIX outMtxView = XMMatrixTranspose(XMLoadFloat4x4(&pmtxCamera));
	//XMMATRIX outMtxProj = XMMatrixTranspose(XMLoadFloat4x4(&pmtxProjection));
	//XMStoreFloat4x4(&pcbWorldViewProMatrix->mWorld, outMtxWorld);
	//XMStoreFloat4x4(&pcbWorldViewProMatrix->mView, outMtxView);
	//XMStoreFloat4x4(&pcbWorldViewProMatrix->mProjection, outMtxProj);
	//VR_ENGINE->GetDeviceContext()->Unmap(m_pcbWorldViewProjMatrix, 0);
	///*ConstantBuffer cb;
	//cb.mWorld = pmtxWorld;
	//cb.mView = pmtxCamera;
	//cb.mProjection = pmtxProjection;
	//VR_ENGINE->GetDeviceContext()->UpdateSubresource(m_pcbWorldViewProjMatrix, 0, nullptr, &cb, 0, 0);*/
	//VR_ENGINE->GetDeviceContext()->VSSetConstantBuffers(VS_CB_SLOT_WORLD_VIEW_PROJECTION_MATRIX, 1, &m_pcbWorldViewProjMatrix);
}

void CEntity::SetPosition(const float x, const float y, const float z, const bool local)
{
	XMFLOAT3 temp = XMFLOAT3(x, y, z);

	if (local)
	{
		XMStoreFloat3(&m_vPosition, XMVector3TransformNormal(XMLoadFloat3(&temp), XMLoadFloat4x4(&m_mtxLocal)));
	}
	else
	{
		m_vPosition = temp;
	}
	// Camera Move?
}

void CEntity::SetPositionY(const float y, const bool local)
{
	XMFLOAT3 temp = XMFLOAT3(GetPosition()->x, y, GetPosition()->z);

	if (local)
	{
		XMStoreFloat3(&m_vPosition, XMVector3TransformNormal(XMLoadFloat3(&temp), XMLoadFloat4x4(&m_mtxLocal)));
	}
	else
	{
		m_vPosition = temp;
	}
}

void CEntity::SetPositionZ(const float z, const bool local)
{
	XMFLOAT3 temp = XMFLOAT3(GetPosition()->x, GetPosition()->y, z);

	if (local)
	{
		XMStoreFloat3(&m_vPosition, XMVector3TransformNormal(XMLoadFloat3(&temp), XMLoadFloat4x4(&m_mtxLocal)));
	}
	else
	{
		m_vPosition = temp;
	}
}

void CEntity::Initalize()
{
	
}

void CEntity::Update(const float fTimeElapsed)
{
	m_mtxLocal._11 = m_vRight.x;    m_mtxLocal._12 = m_vRight.y;	m_mtxLocal._13 = m_vRight.z;
	m_mtxLocal._21 = m_vUp.x;       m_mtxLocal._22 = m_vUp.y;		m_mtxLocal._23 = m_vUp.z;
	m_mtxLocal._31 = m_vLook.x;     m_mtxLocal._32 = m_vLook.y;		m_mtxLocal._33 = m_vLook.z;
	m_mtxLocal._41 = m_vPosition.x; m_mtxLocal._42 = m_vPosition.y; m_mtxLocal._43 = m_vPosition.z;

	Update(m_pParent->GetLocalMatrix());
}

void CEntity::OnPrepareRender()
{
	//VR_ENGINE->GetDeviceContext()->RSSetState(m_pRasterizerState);
	UpdateWorldMatrixConstantBuffer(m_mtxWorld);
	if (m_pMaterial) m_pMaterial->UpdateConstantBuffer();
	if (m_pTexture) m_pTexture->PSSetTextureAndSampler();
}

void CEntity::Render()
{
	OnPrepareRender();
	for (CMesh* pMesh : m_pMeshes)
	{
		if(pMesh) pMesh->Render(VR_ENGINE->GetDeviceContext());
	}
}
