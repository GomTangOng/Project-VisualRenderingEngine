#pragma once

#define SHADER_MANAGER CShaderManager::GetInstance()

class CShader;
class CCamera;
class CShaderManager : public Singleton<CShaderManager>
{
public :
	CShaderManager();
	~CShaderManager();

	bool Initalize(ID3D11Device* pDevice);
	void ShutDown();
	void Update(const float fTimeElapsed);
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT CreateVertexShaderFromFile(WCHAR *pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel,
									   ID3D11VertexShader **ppVertexShader, D3D11_INPUT_ELEMENT_DESC *pInputElements, UINT nElements, 
									   ID3D11InputLayout **ppInputLayout);
	HRESULT CreatePixelShaderFromFile(WCHAR *pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3D11PixelShader **ppd3dPixelShader);
	HRESULT CreateHullShaderFromFile(WCHAR *pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3D11HullShader **ppd3dHullShader);
	HRESULT CreateDomainShaderFromFile(WCHAR *pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3D11DomainShader **ppd3dDomainShader);
	HRESULT CreateComputeShaderFromFile(WCHAR *pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3D11ComputeShader **ppd3dComputeShader);


	ID3D11Buffer* CreateBuffer(  const UINT nStride,
										const int nElements,
										const void * const pBufferData,
										const UINT nBindFlags,
										const D3D11_USAGE d3dUsage,
										const UINT nCPUAccessFlags);
	bool AddShader(const int idx, CShader *pShader);
	void RemoveShader(const int idx);
	void Render();
	void Render(CCamera *pCamera);
private :
	ID3D11Device *m_pDevice;
public :
	unordered_map<int, CShader *> m_mapShaders;
};

