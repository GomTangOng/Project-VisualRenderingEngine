#pragma once
#include "Scene.h"

class CSkyBoxEntity;
class CMyScene : public CScene
{
public:
	CMyScene();
	virtual ~CMyScene();

	virtual bool Initalize();
	virtual void Update(const float fTimeElapsed);
	virtual void BuildObjects();
	virtual void ProcessInput();
private:
	CSkyBoxEntity* m_pSkyBoxEntity;

	float m_fCamSpeed;
public :
	void SetSkyBoxEntity(CSkyBoxEntity* sky) { m_pSkyBoxEntity = sky; }
	CSkyBoxEntity* GetSkyBoxEntity() { return m_pSkyBoxEntity; }
};

