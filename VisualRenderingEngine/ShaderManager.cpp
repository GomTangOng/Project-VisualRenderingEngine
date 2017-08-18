#include "stdafx.h"
#include "ShaderManager.h"
#include "VREngine.h"
#include "Utility.h"
#include "Shader.h"
#include "Entity.h"
CShaderManager::CShaderManager()
{
	
}


CShaderManager::~CShaderManager()
{
	CEntity::ReleaseConctantBuffer();
}

bool CShaderManager::Initalize(ID3D11Device * pDevice)
{
	m_pDevice = pDevice;

	CEntity::InitConstantBuffer();

	return true;
}

void CShaderManager::ShutDown()
{
	for (auto& shader : m_mapShaders) { Memory::Delete(shader.second); } m_mapShaders.clear();
	
}

HRESULT CShaderManager::CreateVertexShaderFromFile(WCHAR * pFileName, LPCSTR pEntryPoint,
												   LPCSTR pShaderModel, ID3D11VertexShader ** ppVertexShader, 
												   D3D11_INPUT_ELEMENT_DESC * pInputElements, UINT nElements, 
												   ID3D11InputLayout ** ppInputLayout)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	ID3DBlob *pVertexShaderBlob = nullptr,  *pErrorBlob = nullptr;
	hr = D3DCompileFromFile(pFileName, nullptr, nullptr, pEntryPoint, pShaderModel,
							dwShaderFlags, 0, &pVertexShaderBlob, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	else
	{
		m_pDevice->CreateVertexShader(pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), nullptr, ppVertexShader);
		m_pDevice->CreateInputLayout(pInputElements, nElements, pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), ppInputLayout);
		
		pVertexShaderBlob->Release();
	}

	return S_OK;
}

HRESULT CShaderManager::CreatePixelShaderFromFile(WCHAR * pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel,
												ID3D11PixelShader ** ppPixelShader)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	ID3DBlob *pPixelShaderBlob = nullptr, *pErrorBlob = nullptr;
	hr = D3DCompileFromFile(pFileName, nullptr, nullptr, pEntryPoint, pShaderModel,
							dwShaderFlags, 0, &pPixelShaderBlob, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	else
	{
		m_pDevice->CreatePixelShader(pPixelShaderBlob->GetBufferPointer(), pPixelShaderBlob->GetBufferSize(), nullptr, ppPixelShader);
	
		pPixelShaderBlob->Release();
	}

	return S_OK;
}

HRESULT CShaderManager::CreateHullShaderFromFile(WCHAR * pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3D11HullShader ** ppd3dHullShader)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	ID3DBlob *pHullShaderBlob = nullptr, *pErrorBlob = nullptr;
	hr = D3DCompileFromFile(pFileName, nullptr, nullptr, pEntryPoint, pShaderModel,
		dwShaderFlags, 0, &pHullShaderBlob, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	else
	{
		m_pDevice->CreateHullShader(pHullShaderBlob->GetBufferPointer(), pHullShaderBlob->GetBufferSize(), nullptr, ppd3dHullShader);

		pHullShaderBlob->Release();
	}
}

HRESULT CShaderManager::CreateDomainShaderFromFile(WCHAR * pFileName, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3D11DomainShader ** ppd3dDomainShader)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	ID3DBlob *pDomainShaderBlob = nullptr, *pErrorBlob = nullptr;
	hr = D3DCompileFromFile(pFileName, nullptr, nullptr, pEntryPoint, pShaderModel,
		dwShaderFlags, 0, &pDomainShaderBlob, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	else
	{
		m_pDevice->CreateDomainShader(pDomainShaderBlob->GetBufferPointer(), pDomainShaderBlob->GetBufferSize(), nullptr, ppd3dDomainShader);

		pDomainShaderBlob->Release();
	}
}


HRESULT CShaderManager::CompileShaderFromFile(WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	
	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
							dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}


ID3D11Buffer * CShaderManager::CreateBuffer(
	const UINT nStride,
	const int nElements,
	const void * const pBufferData,
	const UINT nBindFlags,
	const D3D11_USAGE d3dUsage,
	const UINT nCPUAccessFlags)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = d3dUsage;
	bufferDesc.ByteWidth = nStride * nElements;
	bufferDesc.BindFlags = nBindFlags;
	bufferDesc.CPUAccessFlags = nCPUAccessFlags;
	D3D11_SUBRESOURCE_DATA BufferData;
	ZeroMemory(&BufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	BufferData.pSysMem = pBufferData;

	ID3D11Buffer *pOutBuffer = NULL;
	HR(m_pDevice->CreateBuffer(&bufferDesc, (pBufferData) ? &BufferData : NULL, &pOutBuffer));

	return(pOutBuffer);
}

bool CShaderManager::AddShader(const int idx, CShader * pShader)
{
	if (m_mapShaders[idx])
		return false;
	else
		m_mapShaders[idx] = pShader;
	return true;
}

void CShaderManager::RemoveShader(const int idx)
{
	m_mapShaders.erase(idx);
}

void CShaderManager::Render()
{
	for (auto& shader : m_mapShaders)
	{
		shader.second->Render();
	}
}

void CShaderManager::Render(CCamera * pCamera)
{
	for (auto& shader : m_mapShaders)
	{
		shader.second->Render(pCamera);
	}
}

//ID3D11Buffer * CShaderManager::CreateBuffer(const UINT nStride,
//											const int nElements,
//											const void * const pBufferData,
//											const UINT nBindFlags,
//											const D3D11_USAGE d3dUsage,
//											const UINT nCPUAccessFlags)
//{
//	D3D11_BUFFER_DESC bufferDesc;
//	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
//	bufferDesc.Usage = d3dUsage;
//	bufferDesc.ByteWidth = nStride * nElements;
//	bufferDesc.BindFlags = nBindFlags;
//	bufferDesc.CPUAccessFlags = nCPUAccessFlags;
//	D3D11_SUBRESOURCE_DATA BufferData;
//	ZeroMemory(&BufferData, sizeof(D3D11_SUBRESOURCE_DATA));
//	BufferData.pSysMem = pBufferData;
//
//	ID3D11Buffer *pOutBuffer = NULL;
//	m_pDevice->CreateBuffer(&bufferDesc, (pBufferData) ? &BufferData : NULL, &pOutBuffer);
//
//	return(pOutBuffer);
//}
