#pragma once
#include "Entity.h"

class CTerrainMesh;
class CCamera;
class CTerrainEntity : public CEntity
{
public:
	CTerrainEntity();
	~CTerrainEntity();

	void UpdateTesslationFactor();

	virtual void Initalize() override;
	virtual void Update(const float fTimeElapsed) override;
	virtual void OnPrepareRender() override;
	virtual void Render() override;	

private :
	//CTerrainMesh *m_pMesh;
	XMFLOAT4X4 m_mtxViewProj;
	UINT  m_nWidth;
	UINT  m_nHeight;
	float m_nHeightScale;
	float m_nCellSpacing;
	UINT  m_nPatchVertexRows;
	UINT  m_nPatchVertexCols;
	UINT  m_nPatchVertices;
	UINT  m_nPatchQuadFaces;

	// Contant Variables
	ID3D11Buffer* m_pcbTessBuffer;
	float m_fMinDist{ 20.0f };
	float m_fMaxDist{ 500.0f };
	float m_fMinTess{ 0.0f };
	float m_fMaxTess{ 6.0f };
	float m_fTexelCellSpaceU;
	float m_fTexelCellSpaceV;
	//float m_fWorldCellSpace;
	//XMFLOAT2 m_fTexScale;

	CCamera *m_pCamera;	// to view-frustum

	wstring m_szHeightMapFileName;
	vector<wstring> m_vTextureFileName;
public :
	//CTerrainMesh* GetMesh() { return m_pMesh; }
	float GetWidth() const { return (m_nWidth - 1) * m_nCellSpacing; }		// 이쪽 버그 있는지 확인해볼것
	float GetHeight() const { return (m_nHeight - 1) * m_nCellSpacing; }
	float GetHeightMapWidth()  const { return m_nWidth; }
	float GetHeightMapHeight() const { return m_nHeight; }
	float GetCellSpacing() const { return m_nCellSpacing; }
	UINT GetPatchVertexRows() const { return m_nPatchVertexRows; }
	UINT GetPatchVertexCols() const { return m_nPatchVertexCols; }
	UINT  GetPatchVertices()  const { return m_nPatchVertices; }
	UINT  GetPatchQuadFaces() const { return m_nPatchQuadFaces; }
	//float GetHeight(float x, float z) const { return m_nHeight; }
	float GetHeightScale() const { return m_nHeightScale; }
	wstring& GetHeightMapFileName() { return m_szHeightMapFileName; }

	void SetCamera(CCamera *pCam) { m_pCamera = pCam; }
	void SetPatchVertexRows(const UINT n) { m_nPatchVertexRows = n; }
	void SetPatchVertexCols(const UINT n) { m_nPatchVertexCols = n; }
	void SetPatchVertices(const UINT n) { m_nPatchVertices = n; }
	void SetPatchQuadFaces(const UINT n) { m_nPatchQuadFaces = n; }
};	   
