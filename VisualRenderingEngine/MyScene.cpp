#include "stdafx.h"
#include "MyScene.h"
#include "VREngine.h"
#include "CubeMesh.h"
#include "CubeEntity.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "Tutorial03Shader.h"
#include "InputManager.h"
#include "TerrainManager.h"
#include "ElementType.h"
#include "TerrainShader.h"
#include "TerrainEntity.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"
#include "LigtingExamShader.h"
#include "LightManager.h"
#include "TSTerrainShader.h"
#include "SkyBoxShader.h"
#include "SkyBoxEntity.h"
CMyScene::CMyScene()
{
	m_fCamSpeed = 100.0f;
}


CMyScene::~CMyScene()
{
	//Memory::DeleteVector(m_vecLights);
}

bool CMyScene::Initalize()
{
	CScene::Initalize();
	return true;
}

void CMyScene::Update(const float fTimeElapsed)
{
	if (INPUT_MANAGER->IsStayKeyDown('w') || INPUT_MANAGER->IsStayKeyDown('W'))
		m_pCamera->Walk(m_fCamSpeed * fTimeElapsed);
	if (INPUT_MANAGER->IsStayKeyDown('s') || INPUT_MANAGER->IsStayKeyDown('S'))
		m_pCamera->Walk(-m_fCamSpeed * fTimeElapsed);
	if (INPUT_MANAGER->IsStayKeyDown('a') || INPUT_MANAGER->IsStayKeyDown('A'))
		m_pCamera->Strafe(-m_fCamSpeed * fTimeElapsed);
	if (INPUT_MANAGER->IsStayKeyDown('d') || INPUT_MANAGER->IsStayKeyDown('D'))
		m_pCamera->Strafe(m_fCamSpeed * fTimeElapsed);
	if (INPUT_MANAGER->IsStayKeyDown('q') || INPUT_MANAGER->IsStayKeyDown('Q'))
		m_pCamera->Fly(m_fCamSpeed * fTimeElapsed);
	if (INPUT_MANAGER->IsStayKeyDown('e') || INPUT_MANAGER->IsStayKeyDown('E'))
		m_pCamera->Fly(-m_fCamSpeed * fTimeElapsed);

	LIGHT_MANAGER->UpdateLightConstantBuffer(m_pCamera->GetPosition());

	for (auto& entity : m_mapObjects)
	{
		entity.second->Translate(0.0f, 0.0f, -60 * fTimeElapsed);
		if (entity.second->GetPosition()->z < -200)
		{
			entity.second->SetPositionZ(rand() % 300 + 350);
		}
		entity.second->Update(fTimeElapsed);	
	}
}

#define MAX_OBJECT 518
void CMyScene::BuildObjects()
{
	CSkyBoxShader *pSkyShader = new CSkyBoxShader();
	//CTSTerrainShader *pTSShader = new CTSTerrainShader();
	//CShader *pTerrainShader = new CTerrainShader();
	CShader *TextureShader = new CTutorial03Shader();
	CEntity *pEntity = new CCubeEntity[MAX_OBJECT];
	CLightTexturedCubeMesh *pMesh = new CLightTexturedCubeMesh(2.0f, 2.0f, 2.0f);
	CTerrainEntity *pTerrainEntity = TERRAIN_MANAGER->GetTerrainEntity();
	CSkyBoxEntity *pSkyEntity = new CSkyBoxEntity();
	pSkyEntity->Initalize();
	pSkyShader->AddObject(pSkyEntity);
	m_pCamera->SetChild(pSkyEntity);

	CMaterial *pLandMaterial = new CMaterial();
	CMaterial *pCubeMaterial = new CMaterial();
	//CMaterial *pBoxMaterial = new CMaterial();
	shared_ptr<CDirectionLight> pDirLight[3];
	pDirLight[0] = make_shared<CDirectionLight>(); 
	pDirLight[1] = make_shared<CDirectionLight>();
	pDirLight[2] = make_shared<CDirectionLight>(); 

	shared_ptr<CPointLight> pPointLight = make_shared<CPointLight>();
	shared_ptr<CSpotLight> pSpotLight = make_shared<CSpotLight>();
	pDirLight[0]->m_ambient   = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	pDirLight[0]->m_diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pDirLight[0]->m_speular = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	pDirLight[0]->m_direction = XMFLOAT3(0.707f, -0.707f, 0.0f);
	
	pDirLight[1]->m_ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pDirLight[1]->m_diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	pDirLight[1]->m_speular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	pDirLight[1]->m_direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	pDirLight[2]->m_ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pDirLight[2]->m_diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	pDirLight[2]->m_speular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	pDirLight[2]->m_direction = XMFLOAT3(-0.57735f, -0.57735f, -0.57735f);

	pPointLight->m_ambient     = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	pPointLight->m_diffuse     = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	pPointLight->m_speular     = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	pPointLight->m_attenuation = XMFLOAT3(0.0f, 0.1f, 0.0f);
	pPointLight->m_range = 25.0f;
	pPointLight->m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	pSpotLight->m_ambient     = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pSpotLight->m_diffuse     = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	pSpotLight->m_speular     = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pSpotLight->m_attenuation = XMFLOAT3(1.0f, 0.0f, 0.0f);
	pSpotLight->m_spot = 96.0f;
	pSpotLight->m_range = 25;
	XMFLOAT3 camPos = *m_pCamera->GetPosition();
	//camPos.x += 10;
	pSpotLight->m_position = camPos;
	pSpotLight->m_direction = XMFLOAT3(0.0f, -0.56f, 1.0f);
	
	pCubeMaterial->SetAmbient(XMFLOAT4(0.4f, 0.42f, 0.01f, 1.0f));
	pCubeMaterial->SetDiffuse(XMFLOAT4(0.4f, 0.42f, 0.01f, 1.0f));
	pCubeMaterial->SetSpecular(XMFLOAT4(0.8f, 0.8f, 0.1f, 96.0f));
	
	pLandMaterial->SetAmbient(XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f));
	pLandMaterial->SetDiffuse(XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f));
	pLandMaterial->SetSpecular(XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f));

	/*pMaterial->m_ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	mLandMat.Diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	mLandMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);*/
	LIGHT_MANAGER->AddDirectionLight(pDirLight[0].get());
	LIGHT_MANAGER->AddDirectionLight(pDirLight[1].get());
	LIGHT_MANAGER->AddDirectionLight(pDirLight[2].get());
	//LIGHT_MANAGER->AddPointLight(pPointLight.get());
	//LIGHT_MANAGER->AddSpotLight(pSpotLight.get());
	pSkyShader->BuildObject();
	TextureShader->BuildObject();
	for (int i = 0; i < MAX_OBJECT; ++i)
	{
		pEntity[i].Initalize();
		pEntity[i].AddMesh(pMesh);
		pEntity[i].SetMaterial(pCubeMaterial);
		//if(i%2)
			pEntity[i].SetPosition(rand() % 150 - 50, rand() % 150 - 50, rand() % 300 + 350);
		//else
		//	pEntity[i].SetPosition(10000 - 50, rand() % 100 - 50, rand() % 300 + 200);
		this->AddObject(&pEntity[i]);
		TextureShader->AddObject(&pEntity[i]);
		//VR_ENGINE->AddObject(&pEntity[i]);
		//m_pObjects.push_back(&pEntity[i]);
	}
	//pTerrainEntity->Initalize();
	//pTerrainEntity->SetMaterial(pLandMaterial);
	//pTSShader->BuildObject();
	
	this->SetTerrainObject(pTerrainEntity);
	SHADER_MANAGER->AddShader(0, TextureShader);
	//SHADER_MANAGER->AddShader(1, pTSShader);
	SHADER_MANAGER->AddShader(2, pSkyShader);
	
}

void CMyScene::ProcessInput()
{
	if (INPUT_MANAGER->IsOnceKeyUp('1'))
	{
		VR_ENGINE->SetStreoscopic(true);
		VR_ENGINE->ChangeCameraMode(0);
	}
	
	if (INPUT_MANAGER->IsOnceKeyUp('2'))
	{
		VR_ENGINE->SetStreoscopic(true);
		VR_ENGINE->ChangeCameraMode(CameraDualMode::INTERACE);		// Interace Mode
	}

	if (INPUT_MANAGER->IsOnceKeyUp('3'))
	{
		if(!VR_ENGINE->IsGameStop()) VR_ENGINE->SetGameStop(true);
		else VR_ENGINE->SetGameStop(false);
	}

}
