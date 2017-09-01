#include "stdafx.h"
#include "Camera.h"
#include "VREngine.h"
#include "ShaderManager.h"
#include "SlotType.h"
#include "BufferStructs.h"
#include "SkyBoxEntity.h"
CCamera::CCamera()
{
}


CCamera::~CCamera()
{
	
}

void CCamera::Initalize()
{
	XMMATRIX mtxView;
	XMMATRIX mtxProj;

//	if(!m_pcbViewProjMatrix) m_pcbViewProjMatrix = SHADER_MANAGER->CreateBuffer(sizeof(VS_CB_VIEW_PROJECTION_MATRIX), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	SetPosition(0.0f, 0.0f, 0.0f, false);
	m_vLook = XMFLOAT3(0.0f, 0.0f, 100.0f);
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_vRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_fPitch = m_fYaw = m_fRoll = 0.0f;

	CreateViewMatrix();
	CreateProjectionMatrix(0.01f, 500.0f, (float)VR_ENGINE->GetWindowWidth() / VR_ENGINE->GetWindowHeight(), XM_PI / 3.0f);
	CreateBoundingFrustum();
	//UpdateVertexShaderConstantBuffer(m_mtxView, m_mtxProjection);
}

void CCamera::Update(const float fTimeElapsed)
{
	//m_mtxProjection._31 = 0.015;	// 이게 무슨 의미...?
	//UpdateBoundingFrustum(0.01f, 1000.0f, (float)VR_ENGINE->GetWindowWidth() / VR_ENGINE->GetWindowHeight(), XM_PI / 3.0f);
	//CEntity::Update(fTimeElapsed);
	UpdateViewProjectionConstantBuffer(m_mtxView, m_mtxProjection);
}

int CCamera::IsVisible(CEntity * pEntity)
{
	XMVECTOR detView = XMMatrixDeterminant(XMLoadFloat4x4(&m_mtxView));
	XMMATRIX mtxInverse = XMMatrixInverse(&detView, XMLoadFloat4x4(&m_mtxView));
	//XMVECTOR scale, rot, trans;
	BoundingFrustum out;
	//XMMatrixDecompose(&scale, &rot, &trans, mtxInverse);
	m_boudingFrustum.Transform(out, mtxInverse);
	
	return m_boudingFrustum.Contains(pEntity->GetBoundingBox());
}

void CCamera::Walk(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&m_vLook);
	XMVECTOR p = XMLoadFloat3(&m_vPosition);
	XMStoreFloat3(&m_vPosition, XMVectorMultiplyAdd(s, l, p));
	if(m_pSkyBoxEntity) m_pSkyBoxEntity->SetPosition(m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void CCamera::Strafe(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&m_vRight);
	XMVECTOR p = XMLoadFloat3(&m_vPosition);
	XMStoreFloat3(&m_vPosition, XMVectorMultiplyAdd(s, r, p));
	if(m_pSkyBoxEntity) m_pSkyBoxEntity->SetPosition(m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void CCamera::Fly(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&m_vUp);
	XMVECTOR p = XMLoadFloat3(&m_vPosition);
	XMStoreFloat3(&m_vPosition, XMVectorMultiplyAdd(s, r, p));
	if(m_pSkyBoxEntity) m_pSkyBoxEntity->SetPosition(m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void CCamera::CreateViewMatrix()
{
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(XMLoadFloat3(GetPosition()), XMLoadFloat3(&m_vLook), XMLoadFloat3(&m_vUp)));
}

void CCamera::UpdateViewMatrix()
{
	XMFLOAT3 tmp41, tmp42, tmp43;
	Orthogonalize();
	m_mtxView._11 =  m_vRight.x; m_mtxView._12 = m_vUp.x; m_mtxView._13 = m_vLook.x;
	m_mtxView._21 =  m_vRight.y; m_mtxView._22 = m_vUp.y; m_mtxView._23 = m_vLook.y;
	m_mtxView._31 =  m_vRight.z; m_mtxView._32 = m_vUp.z; m_mtxView._33 = m_vLook.z;
	XMStoreFloat3(&tmp41, XMVector3Dot(XMLoadFloat3(GetPosition()), XMLoadFloat3(&m_vRight)));
	XMStoreFloat3(&tmp42, XMVector3Dot(XMLoadFloat3(GetPosition()), XMLoadFloat3(&m_vUp)));
	XMStoreFloat3(&tmp43, XMVector3Dot(XMLoadFloat3(GetPosition()), XMLoadFloat3(&m_vLook)));
	m_mtxView._41 = -tmp41.x;
	m_mtxView._42 = -tmp42.x;
	m_mtxView._43 = -tmp43.x;
	
	//CreateBoundingFrustum();
	UpdateBoundingFrustum();
	UpdateViewProjectionConstantBuffer(m_mtxView, m_mtxProjection);
}

void CCamera::CreateBoundingFrustum()
{
	static XMVECTOR homogenousPoints[6]
	{
		{ 1.0f,  0.0f, 1.0f, 1.0f },  // 오른쪽 평면
		{ -1.0f,  0.0f, 1.0f, 1.0f }, // 왼쪽 평면
		{ 0.0f,  1.0f, 1.0f, 1.0f },  // 위쪽 평면
		{ 0.0f, -1.0f, 1.0f, 1.0f },  // 아래쪽 평면

		{ 0.0f,  0.0f, 0.0f, 1.0f },  // 가까운 
		{ 0.0f,  0.0f, 1.0f, 1.0f }   // 면 
	};

	XMVECTOR determinant;
	XMMATRIX mtxInverse = XMMatrixInverse(&determinant, XMLoadFloat4x4(&m_mtxProjection));
	XMVECTOR points[6];

	for (int i = 0; i < 6; ++i)
	{
		points[i] = XMVector4Transform(homogenousPoints[i], mtxInverse);
	}

	m_boudingFrustum.Origin = m_vPosition;
	m_boudingFrustum.Orientation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);	// 나중에 바꿔야 한다.

	points[0] = points[0] * XMVectorReciprocal(XMVectorSplatZ(points[0]));
	points[1] = points[1] * XMVectorReciprocal(XMVectorSplatZ(points[1]));
	points[2] = points[2] * XMVectorReciprocal(XMVectorSplatZ(points[2]));
	points[3] = points[3] * XMVectorReciprocal(XMVectorSplatZ(points[3]));

	m_boudingFrustum.RightSlope = XMVectorGetX(points[0]);
	m_boudingFrustum.LeftSlope = XMVectorGetX(points[1]);
	m_boudingFrustum.TopSlope = XMVectorGetY(points[2]);
	m_boudingFrustum.BottomSlope = XMVectorGetY(points[3]);

	points[4] = points[4] * XMVectorReciprocal(XMVectorSplatW(points[4]));
	points[5] = points[5] * XMVectorReciprocal(XMVectorSplatW(points[5]));

	m_boudingFrustum.Near = XMVectorGetZ(points[4]);
	m_boudingFrustum.Far = XMVectorGetZ(points[5]);
}

void CCamera::UpdateBoundingFrustum()
{
	m_boudingFrustum.Origin = m_vPosition;
	//m_boudingFrustum.Orientation = XMFLOAT4(m_fPitch, m_fYaw, m_fRoll, 1.0f); 회전 처리 예정 
}

void CCamera::CreateProjectionMatrix(const float fNear, const float fFar, const float fAspectRatio, const float fFov)
{
 	XMStoreFloat4x4(&m_mtxProjection, XMMatrixPerspectiveFovLH(fFov, fAspectRatio, fNear, fFar));
}

void CCamera::CreateViewport(const DWORD xStart, const DWORD yStart, const DWORD nWidth, const DWORD nHeight, const float fMinZ, const float fMaxZ, const bool isRSSet)
{
	m_viewport.TopLeftX = xStart;
	m_viewport.TopLeftY = yStart;
	m_viewport.Width = nWidth;
	m_viewport.Height = nHeight;
	m_viewport.MinDepth = fMinZ;
	m_viewport.MaxDepth = fMaxZ;

	if (isRSSet) SetViewport();
}

//void CCamera::SetViewport(const DWORD xStart, const DWORD yStart, const DWORD nWidth, const DWORD nHeight, const float fMinZ, const float fMaxZ, const bool isRSSet)
//{
//	
//	m_viewport.TopLeftX = xStart;
//	m_viewport.TopLeftY = yStart;
//	m_viewport.Width = nWidth;
//	m_viewport.Height = nHeight;
//	m_viewport.MinDepth = fMinZ;
//	m_viewport.MaxDepth = fMaxZ;
//
//	//VR_ENGINE->GetDeviceContext()->RSSetViewports(m_viewport);
//}
//}

void CCamera::SetViewport()
{
	VR_ENGINE->GetDeviceContext()->RSSetViewports(1, &m_viewport);
}

void CCamera::ChangeFov(const float fFov)
{
	CreateProjectionMatrix(0.01f, 500.0f, (float)VR_ENGINE->GetWindowWidth() / VR_ENGINE->GetWindowHeight(), XM_PI / 3.0f);
}

void CCamera::SetRightVector(const XMFLOAT3 & vecRight)
{
	m_mtxView._31 = vecRight.x;
	m_mtxView._32 = vecRight.y;
	m_mtxView._33 = vecRight.z;
}

void CCamera::SetUpVector(const XMFLOAT3 & vecUp)
{
	m_mtxView._21 = vecUp.x;
	m_mtxView._22 = vecUp.y;
	m_mtxView._23 = vecUp.z;
}

void CCamera::SetLookVector(const XMFLOAT3 & vecLook)
{
	m_mtxView._11 = vecLook.x;
	m_mtxView._12 = vecLook.y;
	m_mtxView._13 = vecLook.z;
}


