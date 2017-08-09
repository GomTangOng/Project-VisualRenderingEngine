#include "stdafx.h"
#include "Mesh.h"
#include "Utility.h"
#include "ElementType.h"
#include "ShaderManager.h"

struct SimpleVertex
{
	XMFLOAT3 Pos;
};

CMesh::CMesh()
{
	m_nType = VERTEX_POSITION_ELEMENT;
	m_nVertices = 0;
	m_nBuffers = 0;
	m_pPositionBuffer = nullptr;
	m_ppVertexBuffers = nullptr;
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_nSlot = 0;
	m_nStartVertex = 0;
	m_pIndexBuffer = nullptr;
	m_nIndices = 0;
	m_nStartIndex = 0;
	m_nBaseVertex = 0;
	m_nIndexOffset = 0;
	m_dxgiIndexFormat = DXGI_FORMAT_R32_UINT;
	//m_pPositions = nullptr;
	//m_pIndices = nullptr;
}


CMesh::~CMesh()
{
	Memory::Release(m_pPositionBuffer);
	Memory::Release(m_pIndexBuffer);
	Memory::DeleteArray(m_ppVertexBuffers);
	Memory::DeleteArray(m_pnVertexStrides);
	Memory::DeleteArray(m_pnVertexOffsets);
	//Memory::DeleteArray(m_pPositions);
	//Memory::DeleteArray(m_pIndices);
}

void CMesh::AssembleToVertexBuffer(const int nBuffers, 
								   ID3D11Buffer ** ppBuffers, 
								   UINT * pnBufferStrides, 
								   UINT * pnBufferOffsets)
{
	ID3D11Buffer **ppNewVertexBuffers = new ID3D11Buffer*[m_nBuffers + nBuffers];
	UINT *pnNewVertexStrides = new UINT[m_nBuffers + nBuffers];
	UINT *pnNewVertexOffsets = new UINT[m_nBuffers + nBuffers];

	if (m_nBuffers > 0)
	{
		for (int i = 0; i < m_nBuffers; i++)
		{
			ppNewVertexBuffers[i] = m_ppVertexBuffers[i];
			pnNewVertexStrides[i] = m_pnVertexStrides[i];
			pnNewVertexOffsets[i] = m_pnVertexOffsets[i];
		}
		Memory::DeleteArray(m_ppVertexBuffers);
		Memory::DeleteArray(m_pnVertexStrides);
		Memory::DeleteArray(m_pnVertexOffsets);
	}

	for (int i = 0; i < nBuffers; i++)
	{
		ppNewVertexBuffers[m_nBuffers + i] = ppBuffers[i];
		pnNewVertexStrides[m_nBuffers + i] = pnBufferStrides[i];
		pnNewVertexOffsets[m_nBuffers + i] = pnBufferOffsets[i];
	}

	m_nBuffers += nBuffers;
	m_ppVertexBuffers = ppNewVertexBuffers;
	m_pnVertexStrides = pnNewVertexStrides;
	m_pnVertexOffsets = pnNewVertexOffsets;
}

void CMesh::OnPrepareRender(ID3D11DeviceContext * pd3dDeviceContext)
{
	pd3dDeviceContext->IASetVertexBuffers(m_nSlot, m_nBuffers, m_ppVertexBuffers, m_pnVertexStrides, m_pnVertexOffsets);
	if(m_pIndexBuffer)   pd3dDeviceContext->IASetIndexBuffer(m_pIndexBuffer, m_dxgiIndexFormat, m_nIndexOffset);
	pd3dDeviceContext->IASetPrimitiveTopology(m_primitiveTopology);
}

void CMesh::Render(ID3D11DeviceContext * pd3dDeviceContext)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_pIndexBuffer)
		pd3dDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		pd3dDeviceContext->Draw(m_nVertices, m_nStartVertex);
}

void CMesh::RenderInstanced(ID3D11DeviceContext * pd3dDeviceContext, int nInstances, int nStartInstance)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_pIndexBuffer)
		pd3dDeviceContext->DrawIndexedInstanced(m_nIndices, nInstances, m_nStartIndex, m_nBaseVertex, nStartInstance);
	else
		pd3dDeviceContext->DrawInstanced(m_nVertices, nInstances, m_nStartVertex, nStartInstance);
}


// Mesh Manager 만들어서 확장성 있게 만들어야함...

void CMesh::CreateMesh(ID3D11Device *pDevice,
					   const int nVertices, 
					   const D3D11_PRIMITIVE_TOPOLOGY primitiveTopology, 
					   const XMFLOAT3 * const pPosition)
{
	//m_nVertices = nVertices;
	//m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//m_pPositions = new XMFLOAT3[m_nVertices];

	//for (int i = 0; i < m_nVertices; ++i)
	//{
	//	m_pPositions[i] = pPosition[i];
	//}

	//m_pPositionBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	//ID3D11Buffer *pBuffers = m_pPositionBuffer;
	//UINT pBufferStrides = sizeof(XMFLOAT3);
	//UINT pBufferOffsets = 0;
	//AssembleToVertexBuffer(1, &pBuffers, &pBufferStrides, &pBufferOffsets);
}

