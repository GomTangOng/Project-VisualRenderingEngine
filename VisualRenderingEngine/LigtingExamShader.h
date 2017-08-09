#pragma once

#include "Shader.h"

class CLigtingExamShader : public CShader
{
public:
	CLigtingExamShader();
	~CLigtingExamShader() override;

	void CreateShader() override;
	void BuildObject() override;
};

