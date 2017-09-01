#include "stdafx.h"
#include "InstancedCubeShader.h"
#include "ShaderManager.h"
#include "ElementType.h"
#include "Entity.h"
#include "Mesh.h"
#include "VREngine.h"
#include"Camera.h"
#include "CubeMesh.h"
CInstancedCubeShader::CInstancedCubeShader()
{
}


CInstancedCubeShader::~CInstancedCubeShader()
{
}

void CInstancedCubeShader::CreateShader()
{
	BuildInputElementDesc(VertexElementType::VERTEX_POSITION_ELEMENT | 
						  VertexElementType::VERTEX_NORMAL_ELEMENT | 
						  VertexElementType::VERTEX_TEXTURE_ELEMENT_0 | 
						  VertexElementType::VERTEX_INSTANCING_ELEMENT);
	SHADER_MANAGER->CreateVertexShaderFromFile(L"Effect.hlsl", 
											   "VS_INSTANCED_LIGHTING_TEXTURED", 
											   "vs_5_0", 
											   &m_pVertexShader, 
											   m_pInputElementDesc, 
											   m_nInputElements, 
											   &m_pVertexLayout);
	SHADER_MANAGER->CreatePixelShaderFromFile(L"Effect.hlsl",
												"PS_INTANECD_LIGHTING_TEXTURED",
												"ps_5_0",
												&m_pPixelShader);

}

void CInstancedCubeShader::BuildObject()
{
	CreateShader();
}

void CInstancedCubeShader::Update(const float fTimeElapsed)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	CMesh* pMesh = m_pObjects[0]->GetMesh();
	auto buf = static_cast<CInstancedLightTexturedCubeMesh *>(pMesh)->GetInstancedBuffer();
	VR_ENGINE->GetDeviceContext()->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	XMFLOAT4X4 *worldMtx = (XMFLOAT4X4 *)d3dMappedResource.pData;
	//인스턴싱 객체들의 월드 변환 행렬을 정점 버퍼에 쓴다.
	for (int j = 0; j < m_pObjects.size(); ++j)
		worldMtx[j] = *m_pObjects[j]->GetWorldMatrix();
	VR_ENGINE->GetDeviceContext()->Unmap(buf, 0);
}

void CInstancedCubeShader::Render()
{
	OnPreRender();
	Update();
	m_pObjects[0]->OnPrepareRender();
	m_pObjects[0]->GetMesh()->RenderInstanced(VR_ENGINE->GetDeviceContext(), m_pObjects.size(), 0);
}

void CInstancedCubeShader::Render(CCamera * pCamera)
{
	OnPreRender();
	Update();
	for (auto obj : m_pObjects)
	{
		if (pCamera->IsVisible(obj))
			m_nVisibleObjects++;
	}
	m_pObjects[0]->OnPrepareRender();
	m_pObjects[0]->GetMesh()->RenderInstanced(VR_ENGINE->GetDeviceContext(), m_pObjects.size(), 0);
}

void CInstancedCubeShader::OnPreRender()
{
	CShader::OnPreRender();
}
