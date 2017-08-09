#pragma once

#include "Singleton.h"
#include "Light.h"

#define LIGHT_MANAGER CLightManager::GetInstance()

class CLightManager : public Singleton<CLightManager>
{
public:
	CLightManager();
	~CLightManager();

	bool Initalize(ID3D11Device* pDevice);
	void ShotDown();
	void ClearLight();

	void AddDirectionLight(CDirectionLight *pDirLight);
	void AddPointLight(CPointLight *pPointLight);
	void AddSpotLight(CSpotLight *pSpotLight);


	void UpdateLightConstantBuffer(XMFLOAT3 *pCamPos);
private :
	static ID3D11Buffer *m_pLightConstantBuffer;
	ID3D11Device *m_pDevice;

	CDirectionLight *m_ppDirectionLights[MAX_LIGHT];
	int m_nDirLight{ 0 };
	CPointLight  *m_ppPointLights[MAX_LIGHT];
	int m_nPointLight{ 0 };
	CSpotLight *m_ppSpotLights[MAX_LIGHT];
	int m_nSpotLight{ 0 };
};

