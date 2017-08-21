#pragma once

class CEntity;
class CCamera;
class CRenderState;
class CShader
{
public:
	CShader();
	virtual ~CShader();

	void AddObject(CEntity *pEntity);
	void BuildInputElementDesc(const UINT nVertexElementType);
	void GetShaderName(const UINT nVertexElementType, 
						LPCSTR *ppVSShaderName,
						LPCSTR *ppPSShaderName,
						LPCSTR *ppGSSahderName,
						LPCSTR *ppHSSahderName,
						LPCSTR *ppDSSahderName);

	virtual void CreateShader();
	virtual void Render();
	virtual void Render(CCamera *pCamera);
	virtual void OnPreRender();
	virtual void BuildObject() {}

	
protected :
	UINT							m_iVertexElementType;
	int								m_nInputElements;
	D3D11_INPUT_ELEMENT_DESC*       m_pInputElementDesc;

	ID3D11VertexShader*     m_pVertexShader = nullptr;
	ID3D11PixelShader*      m_pPixelShader = nullptr;
	ID3D11InputLayout*      m_pVertexLayout = nullptr;
	ID3D11HullShader*       m_pHullShader = nullptr;
	ID3D11DomainShader*     m_pDomainShader = nullptr;
	//ID3D11Buffer*           m_pVertexBuffer = nullptr;	// temp

	//CRenderState *m_pRenderState;

	// TEMP
	vector<CEntity *> m_pObjects;

	int m_nVisibleObjects;

private :
	
};

