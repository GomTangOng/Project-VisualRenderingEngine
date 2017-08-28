#include "stdafx.h"
#include "OffScreenMesh.h"
#include "ShaderManager.h"
#include "GeometryGenerator.h"

COffScreenMesh::COffScreenMesh(const float width, const float height)
{
	CGeometryGenerator::MeshData m;
	CGeometryGenerator::GetInstance()->CreateFullscreenQuad(m);

	vector<XMFLOAT3> pos(m.Vertices.size());
	vector<UINT> index(m.Indices.size());
	vector<XMFLOAT2> tex(m.Vertices.size());

	for (int i=0; i<m.Vertices.size(); ++i)
	{
		pos[i] = m.Vertices[i].Position;
		tex[i] = m.Vertices[i].TexC;
	}
	for (int i = 0; i < m.Indices.size(); ++i)
	{
		index[i] = m.Indices[i];
	}
	
	m_nIndices = index.size();
	m_nVertices = pos.size();
	m_pPositionBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3), m.Vertices.size(), &pos[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pTexBuffer      = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT2), m.Vertices.size(), &tex[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pIndexBuffer = SHADER_MANAGER->CreateBuffer(sizeof(UINT), m.Indices.size(), &index[0], D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_dxgiIndexFormat = DXGI_FORMAT_R32_UINT;

	ID3D11Buffer* bufs[2]{ m_pPositionBuffer, m_pTexBuffer };
	UINT bufStride[2]{ sizeof(XMFLOAT3) ,sizeof(XMFLOAT2) };
	UINT bufOffset[2]{ 0, 0 };
	AssembleToVertexBuffer(2, bufs, bufStride, bufOffset);
}

COffScreenMesh::~COffScreenMesh()
{
	
}
