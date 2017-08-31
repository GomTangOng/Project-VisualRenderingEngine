#include "stdafx.h"
#include "Scene.h"
#include "Entity.h"
#include "Utility.h"
#include "CNameGenerator.h"
#include "TerrainManager.h"
#include "ShaderManager.h"
#include "SlotType.h"
#include "VREngine.h"
#include "BufferStructs.h"
#include "Shader.h"
CScene::CScene()
{
}


CScene::~CScene()
{
}

void CScene::AddObject(CEntity * pEntity)
{
	m_mapObjects.insert(make_pair(NAME_GENERATOR->Generate(), pEntity));
}


void CScene::AddObject(string name, CEntity * pAddObject)
{
	m_mapObjects.insert(make_pair(name, pAddObject));
}

void CScene::SetTerrainObject(CTerrainEntity * pTerrainEntity)
{
	TERRAIN_MANAGER->SetTerrainEntity(pTerrainEntity);
}

void CScene::RemoveObject(string name)
{
	m_mapObjects.erase(name);
}

//void CScene::AddLight(CLight * pLight)
//{
//	m_vecLights.push_back(pLight);
//}

bool CScene::Initalize()
{
	
	return true;
}

void CScene::BuildObjects()
{
}

void CScene::RemoveAllObjects()
{
	for (auto& obj : m_mapObjects) { Memory::Delete(obj.second); } 
	m_mapObjects.clear();
}

void CScene::Update(const float fTimeElapsed)
{
	//for (auto& shader : SHADER_MANAGER->m_mapShaders) { shader.second->Update(fTimeElapsed); }
	for (auto& obj : m_mapObjects) { obj.second->Update(fTimeElapsed); }
}

void CScene::Render(CCamera * pCamera)
{
	SHADER_MANAGER->Render(pCamera);
}

void CScene::Render()
{
	SHADER_MANAGER->Render();
}

void CScene::ProcessInput()
{
	
}
