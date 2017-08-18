#pragma once

#include "Shader.h"

class CTSTerrainShader : public CShader
{
public:
	CTSTerrainShader();
	~CTSTerrainShader() override;

	void CreateShader() override;
	void BuildObject() override;
	void Render() override;
	void Render(CCamera *pCamera) override;
	void OnPreRender() override;
};

