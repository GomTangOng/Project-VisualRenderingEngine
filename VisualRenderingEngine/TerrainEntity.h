#pragma once
#include "Entity.h"

class CTerrainEntity : public CEntity
{
public:
	CTerrainEntity();
	~CTerrainEntity();

	virtual void Initalize() override;
	virtual void Update(const float fTimeElapsed) override;
	virtual void OnPrepareRender() override;
	virtual void Render() override;	
};

