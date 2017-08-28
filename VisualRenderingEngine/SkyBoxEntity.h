#pragma once
#include "Entity.h"

class CCamera;
class CSkyBoxEntity : public CEntity
{
public:
	CSkyBoxEntity();
	~CSkyBoxEntity() override;

	void Initalize() override;
	void Update(const float fTimeElapsed) override;
	void OnPrepareRender() override;
	void Render() override;
};

