#pragma once

#include "Shader.h"

class CTutorial02Shader : public CShader
{
public:
	CTutorial02Shader();
	~CTutorial02Shader() override;
	
	void CreateShader() override;
	void BuildObject() override;
	void Render() override;
	void OnPreRender() override;
private :

};

