#pragma once

#include "Entity.h"

class CPlayer;
class CSkyBoxEntity;
class CCamera : public CEntity
{
public:
	CCamera();
	~CCamera();

	virtual void Initalize();
	virtual void Update(const float fTimeElapsed);
	int IsVisible(CEntity *pEntity);

	void Walk(float d);	// Z축 이동
	void Strafe(float d);	// X축 이동
	void Fly(float d);

	void CreateViewMatrix();
	void UpdateViewMatrix();
	void CreateBoundingFrustum();
	void UpdateBoundingFrustum();	// 나중에 작업
	void CreateProjectionMatrix(const float fNear, const float fFar, const float fAspectRatio, const float fFov);
	void CreateViewport(const DWORD xStart, const DWORD yStart, const DWORD nWidth, const DWORD nHeight, const float fMinZ = 0.0f, const float fMaxZ = 1.0f, const bool isRSSet = true);
	void SetViewport(const DWORD xStart, const DWORD yStart, const DWORD nWidth, const DWORD nHeight, const float fMinZ = 0.0f, const float fMaxZ = 1.0f);
	void SetViewport();
	void ChangeFov(const float fFov);
private :
	XMFLOAT4X4 m_mtxView;
	XMFLOAT4X4 m_mtxProjection;
	BoundingFrustum m_boudingFrustum;

	float m_fNearZ;
	float m_fFarZ;
	float m_fAspect;
	float m_fFov;
	float m_fNearWindowHeight;
	float m_fFarWindowHeight;
	D3D11_VIEWPORT m_viewport;

	BYTE m_iMode;
	CPlayer*       m_pPlayer{ nullptr };
	CSkyBoxEntity* m_pSkyBoxEntity{ nullptr };
	bool m_bInterace = false;
public :
	void SetSkyBoxEntity(CSkyBoxEntity *pEntity) { m_pSkyBoxEntity = pEntity; }
	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }
	void SetRightVector(const XMFLOAT3& vecRight);
	void SetUpVector(const XMFLOAT3& vecUp);
	void SetLookVector(const XMFLOAT3& vecLook);
	void SetInterace(const bool interace) { m_bInterace = interace; }

	CSkyBoxEntity* GetSkyBoxEntity() { return m_pSkyBoxEntity; }
	CPlayer* Getplayer() { return m_pPlayer; }
	XMFLOAT3 GetLookVector() { return XMFLOAT3(m_mtxView._11, m_mtxView._12, m_mtxView._13); }
	XMFLOAT3 GetUpVector() { return XMFLOAT3(m_mtxView._21, m_mtxView._22, m_mtxView._23); }
	XMFLOAT3 GetRightVector() { return XMFLOAT3(m_mtxView._31, m_mtxView._32, m_mtxView._33); }

	XMVECTOR    GetRightVectorXM() { return XMLoadFloat3(&GetRightVector()); }
	XMVECTOR    GetUpVectorXM() { return XMLoadFloat3(&GetUpVector()); }
	XMVECTOR    GetLookVectorXM() { return XMLoadFloat3(&GetLookVector()); }
	XMFLOAT4X4& GetProjectionMatrix() { return m_mtxProjection; }
	XMFLOAT4X4& GetViewMatrix() { return m_mtxView; }
	XMMATRIX    GetProjectionMatrixXM() { return XMLoadFloat4x4(&m_mtxProjection); }
	XMMATRIX    GetViewMatrixXM() { return XMLoadFloat4x4(&m_mtxView); }
	XMMATRIX    GetViewProjectionMatrixXM() { return XMMatrixMultiply(GetProjectionMatrixXM(), GetViewMatrixXM()); }
	bool IsInterace() { return m_bInterace; }
	
};

