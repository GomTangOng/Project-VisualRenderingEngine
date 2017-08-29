#pragma once

#include "Shader.h"

class COffScreenMesh;
class COffScreenRenderShader : public CShader
{
public:
	COffScreenRenderShader();
	~COffScreenRenderShader() override;

	void CreateShader() override;
	void BuildObject() override;
	void Render() override;
	void OnPreRender() override;
private :
	COffScreenMesh *m_pRenderMesh;
};

