#include "stdafx.h"
#include "TerrainManager.h"
#include "GeometryGenerator.h"
#include "TerrainMesh.h"
#include "ShaderManager.h"
#include "Utility.h"

CTerrainManager::CTerrainManager()
{
}


CTerrainManager::~CTerrainManager()
{

}

bool CTerrainManager::Initalize(ID3D11Device * pDevice)
{
	m_pDevice = pDevice;

	BuildTerrain(320, 320, 50, 50);

	return true;
}

void CTerrainManager::ShutDown()
{
	Memory::DeleteVector(m_vecMeshes);
	Memory::DeleteVector(m_vecTerrainEntity);
}

void CTerrainManager::BuildTerrain(const float width, const float depth, UINT m, UINT n)
{
	CTerrainMesh* pMesh = new CTerrainMesh();

	m_vecMeshes.push_back(pMesh);
	
	CGeometryGenerator::MeshData grid;
	CGeometryGenerator geoGen;
	geoGen.CreateGrid(width, depth, m, n, grid);
	UINT vertices_cnt = grid.Vertices.size();
	UINT indices_cnt = grid.Indices.size();

	m_vecMeshes.reserve(vertices_cnt);
	
	vector<XMFLOAT3> positions(vertices_cnt);
	vector<XMFLOAT3> normals(vertices_cnt);
	for (int i=0; i<vertices_cnt; ++i)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;
		p.y = GetHeight(p.x, p.z);
		positions[i] = p;
		normals[i] = GetHillNormal(p.x, p.z);
		//if (p.y < -10.0f)
		//{
		//	// Sandy beach color.
		//	colors[i] = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
		//}
		//else if (p.y < 5.0f)
		//{
		//	// Light yellow-green.
		//	colors[i] = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
		//}
		//else if (p.y < 12.0f)
		//{
		//	// Dark yellow-green.
		//	colors[i] = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
		//}
		//else if (p.y < 20.0f)
		//{
		//	// Dark brown.
		//	colors[i] = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
		//}
		//else
		//{
		//	// White snow.
		//	colors[i] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//}
	}

	ID3D11Buffer* pPositionBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3), vertices_cnt, &positions[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
	ID3D11Buffer* pNormalsBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT4), vertices_cnt, &normals[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
	ID3D11Buffer* pIndicesBuffer = SHADER_MANAGER->CreateBuffer(sizeof(UINT), indices_cnt, &grid.Indices[0], D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
	ID3D11Buffer* pBuffers[2]{ pPositionBuffer, pNormalsBuffer };
	UINT pBufferStrides[2]{ sizeof(XMFLOAT3), sizeof(XMFLOAT3) };
	UINT pBufferOffset[2]{ 0, 0 };

	pMesh->SetVertexCount(vertices_cnt);
	pMesh->SetIndexCount(indices_cnt);
	pMesh->SetDXGIIndexFormat(DXGI_FORMAT_R32_UINT);
	pMesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pMesh->SetPositionBuffer(pPositionBuffer);
	pMesh->SetIndexBuffer(pIndicesBuffer);
	pMesh->SetNoramlConstantBuffer(pNormalsBuffer);
	//pMesh->SetColorBuffer(pColorsBuffer);
	pMesh->AssembleToVertexBuffer(2, pBuffers, pBufferStrides, pBufferOffset);
}

float CTerrainManager::GetHeight(const float x, const float z)
{
	return 0.3f * ( z * sinf(0.1f * x) + x * cosf(0.1f * z) );
}

XMFLOAT3 CTerrainManager::GetHillNormal(float x, float z) const
{
	// n = (-df/dx, 1, -df/dz)
	XMFLOAT3 n(
		-0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z),
		1.0f,
		-0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z));

	XMVECTOR unitNormal = XMVector3Normalize(XMLoadFloat3(&n));
	XMStoreFloat3(&n, unitNormal);

	return n;
}