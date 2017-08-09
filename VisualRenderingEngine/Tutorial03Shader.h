#pragma once

#include "Shader.h"

class CTutorial03Shader : public CShader
{
public:
	CTutorial03Shader();
	~CTutorial03Shader() override;

	void CreateShader() override;
	void BuildObject() override;
	void Render() override;
	void OnPreRender() override;
};

