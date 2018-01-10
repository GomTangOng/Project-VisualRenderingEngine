#pragma once
#include "Singleton.h"

#define TERRAIN_MANAGER CTerrainManager::GetInstance()

class CTerrainMesh;
class CTerrainEntity;
class CTexture;
class CTerrainManager : public Singleton<CTerrainManager>
{
public:
	CTerrainManager();
	~CTerrainManager();

	bool Initalize(ID3D11Device* pDevice);
	void ShutDown();
	void BuildQuadPatchVB();
	void BuildQuadPatchIB();
	void BuildHeightmapSRV();

	bool LoadHeightMap(CTerrainEntity *pTerrain);
	void BuildTerrain(const float width, const float depth, UINT m, UINT n);	// no heightmap
	void Smooth();
	void CalcAllPatchBoundsY();
	void CalcPatchBoundsY(const UINT x, const UINT z);
	bool InBounds(const int x, const int z);
	float Average(const int x, const int z);

	float GetSimpleHeight(const float x, const float z);
	XMFLOAT3 GetHillNormal(float x, float z)const;
private :
	static const int s_nCellsPerPatch = 64;
	
	vector<float>    m_vHeightMap;
	vector<XMFLOAT2> m_vPatchBoundsY;
	CTerrainEntity*  m_pTerrainEntity;
	
	ID3D11Device* m_pDevice;
	CTexture*     m_pHeightMap;
	//ID3D11ShaderResourceView *m_pHeightMapSRV;

	vector<CTerrainMesh *> m_vMeshes;	// for simple terrain
public :
	void SetTerrainEntity(CTerrainEntity* pEntity) { m_pTerrainEntity = pEntity; }

	float GetHeight(const float x, const float z);
	CTerrainEntity* GetTerrainEntity() { return m_pTerrainEntity; }
	CTexture* GetHeightMap() { return m_pHeightMap; }
	vector<CTerrainMesh *>& GetMeshes() { return m_vMeshes; }
};

