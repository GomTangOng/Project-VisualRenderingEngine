#pragma once


class CShader;
class CMesh;
class CMaterial;
class CTexture;
class CEntity
{
public:
	CEntity();
	~CEntity();

	//virtual void CreateDepthStencilState(ID3D11Device *pd3dDevice) { }
	//virtual void CreateBlendState(ID3D11Device *pd3dDevice) { }
	void Translate(const float x, const float y, const float z, const bool local = true);
	void Rotate(const float x, const float y, const float z);
	void RotationX(const float angle);
	void RotationY(const float angle);
	void RotationZ(const float angle);
	void ScaleX(const float scaleX);
	void ScaleY(const float scaleY);
	void ScaleZ(const float scaleZ);
	void ScaleXYZ(const float scaleX, const float scaleY, const float scaleZ);

	void Orthogonalize();
	void Update(XMFLOAT4X4 *pMtxParent);
	void UpdateBoundingBox();

	void AddMesh(CMesh *pMesh) { m_pMeshes.push_back(pMesh); }
	void InitBoudingBox(XMFLOAT3& centerPos, XMFLOAT3& extents);

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	
	virtual void Initalize();
	virtual void Update(const float fTimeElapsed);
	virtual void OnPrepareRender();
	virtual void Render();
	//static void UpdatePixelShaderConstantBuffer(ID3D11Device *pDevice);
	
	static void InitConstantBuffer();
	static void ReleaseConctantBuffer();
	static void UpdateWorldMatrixConstantBuffer(const XMFLOAT4X4 &pmtxWorld);
	static void UpdateViewProjectionConstantBuffer(XMFLOAT4X4 &pmtxCamera, XMFLOAT4X4 &pmtxProjection);
	static void UpdateVertexShaderConstantBuffer(XMFLOAT4X4 &pmtxWorld, XMFLOAT4X4 &pmtxCamera, XMFLOAT4X4 &pmtxProjection);
protected :
	XMFLOAT4X4 m_mtxLocal;
	XMFLOAT4X4 m_mtxWorld;
	//XMFLOAT4X4 m_mtxView;
	//XMFLOAT4X4 m_mtxProjection;
	BoundingBox m_boundingBox;

	string   m_name;
	XMFLOAT3 m_vPosition;
	XMFLOAT3 m_vRight;
	XMFLOAT3 m_vUp;
	XMFLOAT3 m_vLook;

	float m_fPtich;
	float m_fYaw;
	float m_fRoll;

	CMaterial *m_pMaterial{ nullptr };
	CTexture  *m_pTexture{ nullptr };
	CShader   *m_pShader{ nullptr };
	
	ID3D11RasterizerState			*m_pRasterizerState;
	ID3D11DepthStencilState			*m_pDepthStencilState;
	ID3D11BlendState				*m_pBlendState;

	static ID3D11Buffer*			m_pcbWorldMatrix;
	//static ID3D11Buffer*			m_pcbMaterialColors;
	static ID3D11Buffer*            m_pcbViewProjMatrix;
	//static ID3D11Buffer*			m_pcbWorldViewProjMatrix;
protected :
	vector<CMesh *> m_pMeshes;
	CEntity *m_pParent;
	CEntity *m_pChild;

	bool m_bActive;
	bool m_bVisible;
public :
	void SetParent(CEntity* const pParent) { m_pParent = pParent; }
	void SetChild(CEntity* const pChild) { m_pChild = pChild; }
	void SetActive(const bool active) { m_bActive = active; }
	void SetVisible(const bool visible) { m_bVisible = visible; }
	void SetPosition(const float x, const float y, const float z, const bool local = false);
	void SetPositionY(const float y, const bool local = false);
	void SetPositionZ(const float z, const bool local = false);
	void SetMaterial(CMaterial *pMaterial) { m_pMaterial = pMaterial; }
	void SetTexture(CTexture *pTex) { m_pTexture = pTex; }
	//void SetScale(const float scaleX, const float scaleY, const float scaleZ);
	bool IsActive() const { return m_bActive; }
	bool IsVisible() const { return m_bVisible; }

	CMesh* GetMesh(const UINT idx = 0) { return m_pMeshes[idx]; }
	XMFLOAT4X4* GetLocalMatrix() { return &m_mtxLocal; }
	XMFLOAT3* GetPosition() { return &m_vPosition; }
	XMVECTOR GetPositionXM() { return XMLoadFloat3(&m_vPosition); }
	BoundingBox& GetBoundingBox() { return m_boundingBox; }
	//XMFLOAT3 GetPosition() const { return XMFLOAT3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43); }
};

