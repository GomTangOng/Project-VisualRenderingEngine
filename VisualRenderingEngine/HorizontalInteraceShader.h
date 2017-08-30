#pragma once

#include "Shader.h"


class CHorizontalInteraceShader : public CShader
{
public:
	CHorizontalInteraceShader();
	~CHorizontalInteraceShader() override;

	void CreateShader() override;
	void BuildObject() override;
	void Render() override;
	void OnPreRender() override;

	void BuildViews(const UINT width, const UINT height);
	void UpdateConstantBuffer();

	//ID3D11ShaderResourceView*  m_pOutSRV;
	ID3D11Buffer* m_pcbWindowSizeBuffer;
	ID3D11RenderTargetView*    m_pOutRTV{ nullptr };
	ID3D11ShaderResourceView*  m_pOutSRV{ nullptr };
	ID3D11UnorderedAccessView* m_pOutUAV{ nullptr };
};

