#pragma once
#include "Shader.h"

class CCamera;
class CTerrainShader : public CShader
{
public:
	CTerrainShader();
	~CTerrainShader() override;

	void CreateShader() override;
	void BuildObject() override;
	void Render() override;
	void Render(CCamera *pCamera) override;
	void OnPreRender() override;

private :
};

