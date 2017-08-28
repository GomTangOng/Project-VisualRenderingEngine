#pragma once

#include "Shader.h"
class COffScreenMesh;
class CHorizontalInteraceShader : public CShader
{
public:
	CHorizontalInteraceShader();
	~CHorizontalInteraceShader() override;

	void CreateShader() override;
	void BuildObject() override;
	void Render() override;
	void OnPreRender() override;

	COffScreenMesh *m_pRenderMesh;
};

