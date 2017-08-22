#include "stdafx.h"
#include "Shader.h"
#include "ElementType.h"
#include "VREngine.h"
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
CShader::CShader()
{
}


CShader::~CShader()
{
	Memory::Release(m_pVertexShader);
	Memory::Release(m_pPixelShader);
	Memory::Release(m_pVertexLayout);
	Memory::Release(m_pHullShader);
	Memory::Release(m_pDomainShader);
	// TEMP
	Memory::DeleteVector(m_pObjects);
}

void CShader::AddObject(CEntity * pEntity)
{
	m_pObjects.push_back(pEntity);
}
// Name : GetInputElementDesc(UINT nVertexElementType)
// Desc : 정점의 특성을 인자값으로 넣어주면 그 특성에 맞게 Input Layout을 맞춰준다.


void CShader::BuildInputElementDesc(const UINT nVertexElementType)
{
	m_iVertexElementType = nVertexElementType;
	m_nInputElements = 0;

	if (nVertexElementType & VERTEX_POSITION_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_COLOR_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_NORMAL_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_0) m_nInputElements++;
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_1) m_nInputElements++;
	if (nVertexElementType & VERTEX_BONE_ID_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_BONE_WEIGHT_ELEMENT) m_nInputElements++;
	if (nVertexElementType & VERTEX_INSTANCING_ELEMENT) m_nInputElements += 4;
	m_pInputElementDesc = new D3D11_INPUT_ELEMENT_DESC[m_nInputElements];
	UINT nIndex = 0, nSlot = 0;

	if (nVertexElementType & VERTEX_POSITION_ELEMENT)    m_pInputElementDesc[nIndex++] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_COLOR_ELEMENT)       m_pInputElementDesc[nIndex++] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_NORMAL_ELEMENT)      m_pInputElementDesc[nIndex++] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_0)   m_pInputElementDesc[nIndex++] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_TEXTURE_ELEMENT_1)   m_pInputElementDesc[nIndex++] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_BONE_ID_ELEMENT)     m_pInputElementDesc[nIndex++] = { "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_SINT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_BONE_WEIGHT_ELEMENT) m_pInputElementDesc[nIndex++] = { "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot++, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	if (nVertexElementType & VERTEX_INSTANCING_ELEMENT)
	{
		m_pInputElementDesc[nIndex++] = { "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		m_pInputElementDesc[nIndex++] = { "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		m_pInputElementDesc[nIndex++] = { "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		m_pInputElementDesc[nIndex++] = { "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, nSlot, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
	}
}

void CShader::GetShaderName(const UINT nVertexElementType, 
								  LPCSTR * ppVSShaderName, 
								  LPCSTR * ppPSShaderName, 
	   							  LPCSTR *ppGSSahderName,
								  LPCSTR *ppHSSahderName,
								  LPCSTR *ppDSSahderName)
{
	//ppVSShaderName = ppPSShaderName = ppGSSahderName = ppHSSahderName = ppHSSahderName = ppDSSahderName = nullptr;

	if (nVertexElementType == (VERTEX_POSITION_ELEMENT)) {*ppVSShaderName = "VS", *ppPSShaderName = "PS"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_COLOR_ELEMENT)) { *ppVSShaderName = "VS2", *ppPSShaderName = "PS2"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_NORMAL_ELEMENT)) { *ppVSShaderName = "VS_LIGHTING", *ppPSShaderName = "PS_LIGHTING"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_NORMAL_ELEMENT)) { *ppVSShaderName = "VS_LIGHTING_TEXTURED", *ppPSShaderName = "PS_LIGHTING_TEXTURED"; }
	if (nVertexElementType == (VERTEX_POSITION_ELEMENT | VERTEX_TEXTURE_ELEMENT_0 | VERTEX_TEXTURE_ELEMENT_1)) { *ppVSShaderName = "VS_TERRAIN", *ppPSShaderName = "PS_TERRAIN"; *ppHSSahderName = "HS_TERRAIN"; *ppDSSahderName = "DS_TERRAIN"; }
}

void CShader::CreateShader()
{
	return;
}

void CShader::Render()
{
	OnPreRender();
	for (CEntity *obj : m_pObjects)
	{
		obj->Render();
	}
}

void CShader::Render(CCamera * pCamera)
{
	OnPreRender();
	for (CEntity *obj : m_pObjects)
	{
		if (pCamera->IsVisible(obj))
		{
			obj->Render();
			m_nVisibleObjects++;
		}
	}
}

void CShader::OnPreRender()
{
	if(m_pVertexLayout) VR_ENGINE->GetDeviceContext()->IASetInputLayout(m_pVertexLayout);
	if(m_pVertexShader) VR_ENGINE->GetDeviceContext()->VSSetShader(m_pVertexShader, NULL, 0);
	if(m_pPixelShader) VR_ENGINE->GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, 0);
	if(m_pHullShader) VR_ENGINE->GetDeviceContext()->HSSetShader(m_pHullShader, NULL, 0);
	if(m_pDomainShader) VR_ENGINE->GetDeviceContext()->DSSetShader(m_pDomainShader, NULL, 0);

	m_nVisibleObjects = 0;
}
