#pragma once
#include "Scene.h"
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
	float m_fCamSpeed;
};

