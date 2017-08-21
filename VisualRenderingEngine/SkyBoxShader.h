#pragma once

#include "Shader.h"
class CMesh;
class CTexture;
class CSkyBoxShader : public CShader
{
public:
	CSkyBoxShader();
	~CSkyBoxShader() override;

	void CreateShader() override;
	void BuildObject() override;
	void Render() override;
	void Render(CCamera *pCamera) override;
	void OnPreRender() override;

};

