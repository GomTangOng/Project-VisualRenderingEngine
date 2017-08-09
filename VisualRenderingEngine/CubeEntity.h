#pragma once

#include "Entity.h"

class CCubeEntity : public CEntity
{
public:
	CCubeEntity();
	~CCubeEntity();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice) override;

	virtual void Initalize() override;
	virtual void Update(const float fTimeElapsed) override;
	virtual void OnPrepareRender() override;
	virtual void Render() override;
private :
};

