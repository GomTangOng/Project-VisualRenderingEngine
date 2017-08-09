#pragma once
#include "Singleton.h"

#define TERRAIN_MANAGER CTerrainManager::GetInstance()


class CTerrainMesh;
class CTerrainEntity;
class CTerrainManager : public Singleton<CTerrainManager>
{
public:
	CTerrainManager();
	~CTerrainManager();

	bool Initalize(ID3D11Device* pDevice);
	void ShutDown();
	void BuildTerrain(const float width, const float depth, UINT m, UINT n);

	void AddEntity(CTerrainEntity* pEntity) { m_vecTerrainEntity.push_back(pEntity); }

	float GetHeight(const float x, const float z);
	XMFLOAT3 GetHillNormal(float x, float z)const;
private :
	ID3D11Device* m_pDevice;
	vector<CTerrainEntity*> m_vecTerrainEntity;
	vector<CTerrainMesh *> m_vecMeshes;
public :
	vector<CTerrainMesh *>& GetMeshes() { return m_vecMeshes; }
};

