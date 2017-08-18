#pragma once

#include "Light.h"

class CEntity;
class CCamera;
class CTerrainEntity;

class CScene
{
public:
	CScene();
	virtual ~CScene();


	void AddObject(string name, CEntity *pAddObject);
	void RemoveObject(string name);
	//void AddLight(CLight *pLight);

	virtual bool Initalize();
	virtual void BuildObjects();
	virtual void RemoveAllObjects();
	virtual void Update(const float fTimeElapsed);
	virtual void Render(CCamera *pCamera);
	virtual void Render();
	virtual void ProcessInput();
	virtual void AddObject(CEntity *pEntity);

protected:
	string m_name;
	CCamera *m_pCamera;
	
	//vector<CTerrainEntity *> m_vecTerrainEntity;
	map<string, CEntity *> m_mapObjects;
	//vector<CLight *> m_vecLights;

	vector<CDirectionLight *> m_vecDirLight;
	vector<CPointLight *>     m_vecPointLight;
	vector<CSpotLight *>      m_vecSpotLight;
	//CDirectionLight *m_pDirLight[MAX_LIGHT];
	//CPointLight *m_pPointLight[MAX_LIGHT];
	//CSpotLight *m_pSpotLight[MAX_LIGHT];

	CEntity *m_pSelectedObject;	
public:
	void SetTerrainObject(CTerrainEntity *pTerrainEntity);
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }

	CCamera* GetCamera() { return m_pCamera; }
};

