#include "stdafx.h"
#include "CubeMesh.h"
#include "Mesh.h"
#include "VREngine.h"
#include "ShaderManager.h"
#include "VertexStructs.h"
#include "GeometryGenerator.h"

CDiffusedCubeMesh::CDiffusedCubeMesh()
{
	XMFLOAT4 colors[8];
	
	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
	};
	UINT indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};
	m_nVertices = ARRAYSIZE(vertices);
	m_nIndices = ARRAYSIZE(indices);
	m_dxgiIndexFormat = DXGI_FORMAT_R32_UINT;
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//m_pPositions = new XMFLOAT3[m_nVertices];
	//m_pIndices = new UINT[m_nIndices];

	vector<XMFLOAT3> m_pPositions(m_nVertices);
	vector<UINT> m_pIndices(m_nIndices);
	for (int i = 0; i < m_nVertices; ++i)
	{
		m_pPositions[i] = vertices[i].Pos;
		colors[i] = vertices[i].Color;	
	}
	for (int i = 0; i < m_nIndices; ++i)
	{
		m_pIndices[i] = indices[i];
	}
	// 밑에꺼 자동화 할 수 있게 해보자..
	m_pPositionBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3), m_nVertices, &m_pPositions[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pColorBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT4), m_nVertices, colors, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pIndexBuffer = SHADER_MANAGER->CreateBuffer(sizeof(UINT), m_nIndices, &m_pIndices[0], D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);

	ID3D11Buffer* ppBuffers[2] = { m_pPositionBuffer, m_pColorBuffer };
	UINT pBufferStirdes[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT4) };
	UINT pBufferOffsets[2] = { 0, 0};

	AssembleToVertexBuffer(2, ppBuffers, pBufferStirdes, pBufferOffsets);
}

CDiffusedCubeMesh::~CDiffusedCubeMesh()
{
	CDiffusedMesh::~CDiffusedMesh();
}

void CDiffusedCubeMesh::OnPrepareRender(ID3D11DeviceContext * pd3dDeviceContext)
{
	CMesh::OnPrepareRender(pd3dDeviceContext);
}

void CDiffusedCubeMesh::Render(ID3D11DeviceContext * pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}

void CDiffusedCubeMesh::RenderInstanced(ID3D11DeviceContext * pd3dDeviceContext, int nInstances, int nStartInstance)
{
	CMesh::RenderInstanced(pd3dDeviceContext, nInstances, nStartInstance);
}


CCubeMesh::CCubeMesh(const float width, const float height, const float depth)
{
	//Vertex v[24];

	//float w2 = 0.5f*width;
	//float h2 = 0.5f*height;
	//float d2 = 0.5f*depth;

	//// Fill in the front face vertex data.
	//v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	//v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	//// Fill in the back face vertex data.
	//v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	//v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	//v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	//// Fill in the top face vertex data.
	//v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	//v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	//// Fill in the bottom face vertex data.
	//v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	//v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	//v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	//v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	//// Fill in the left face vertex data.
	//v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	//v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	//v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	//v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	//// Fill in the right face vertex data.
	//v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	//v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	//v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	//v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	////
	//// Create the indices.
	////

	//UINT i[36];

	//// Fill in the front face index data
	//i[0] = 0; i[1] = 1; i[2] = 2;
	//i[3] = 0; i[4] = 2; i[5] = 3;

	//// Fill in the back face index data
	//i[6] = 4; i[7] = 5; i[8] = 6;
	//i[9] = 4; i[10] = 6; i[11] = 7;

	//// Fill in the top face index data
	//i[12] = 8; i[13] = 9; i[14] = 10;
	//i[15] = 8; i[16] = 10; i[17] = 11;

	//// Fill in the bottom face index data
	//i[18] = 12; i[19] = 13; i[20] = 14;
	//i[21] = 12; i[22] = 14; i[23] = 15;

	//// Fill in the left face index data
	//i[24] = 16; i[25] = 17; i[26] = 18;
	//i[27] = 16; i[28] = 18; i[29] = 19;

	//// Fill in the right face index data
	//i[30] = 20; i[31] = 21; i[32] = 22;
	//i[33] = 20; i[34] = 22; i[35] = 23;

	//m_nVertices = ARRAYSIZE(v);
	//m_nIndices = ARRAYSIZE(i);
	//m_dxgiIndexFormat = DXGI_FORMAT_R32_UINT;
	//m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//m_pPositions = new XMFLOAT3[m_nVertices];
	//m_pIndices = new UINT[m_nIndices];
	//for (int i = 0; i < m_nVertices; ++i)
	//{
	//	m_pPositions[i] = vertices[i].Pos;
	//	colors[i] = vertices[i].Color;
	//}
	//for (int i = 0; i < m_nIndices; ++i)
	//{
	//	m_pIndices[i] = indices[i];
	//}
	//m_pPositionBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//m_pColorBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT4), m_nVertices, colors, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	//m_pIndexBuffer = SHADER_MANAGER->CreateBuffer(sizeof(UINT), m_nIndices, m_pIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);

	//ID3D11Buffer* ppBuffers[2] = { m_pPositionBuffer, m_pColorBuffer };
	//UINT pBufferStirdes[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT4) };
	//UINT pBufferOffsets[2] = { 0, 0 };

	//AssembleToVertexBuffer(2, ppBuffers, pBufferStirdes, pBufferOffsets);
}

CCubeMesh::~CCubeMesh()
{
}

CLightCubeMesh::CLightCubeMesh(const float width, const float height, const float depth)
{
	CGeometryGenerator::MeshData mesh;
	CGeometryGenerator::GetInstance()->CreateBox(width, height, depth, mesh);
	m_nVertices = mesh.Vertices.size();
	m_nIndices = mesh.Indices.size();

	//vector<LightVertex> vertices(vertex_cnt);
	vector<UINT> indices(m_nIndices);
	vector<XMFLOAT3> pos(m_nVertices);
	vector<XMFLOAT3> normal(m_nVertices);

	for(int i=0; i<m_nVertices; ++i)
	{
		pos[i]    = mesh.Vertices[i].Position;
		normal[i] = mesh.Vertices[i].Normal;
	}
	for (UINT i = 0; i < m_nIndices; ++i)
	{
		indices[i] = mesh.Indices[i];
	}
	 
	m_dxgiIndexFormat = DXGI_FORMAT_R32_UINT;
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pPositionBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3), m_nVertices, &pos[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pNormalBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3), m_nVertices, &normal[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pIndexBuffer = SHADER_MANAGER->CreateBuffer(sizeof(UINT), m_nIndices, &indices[0], D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);

	ID3D11Buffer* ppBuffers[2] = { m_pPositionBuffer, m_pNormalBuffer };
	UINT pBufferStirdes[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3) };
	UINT pBufferOffsets[2] = { 0, 0 };

	AssembleToVertexBuffer(2, ppBuffers, pBufferStirdes, pBufferOffsets);
}

CLightCubeMesh::~CLightCubeMesh()
{
	Memory::Release(m_pNormalBuffer);
}

CLightTexturedCubeMesh::CLightTexturedCubeMesh(const float width, const float height, const float depth)
{
	CGeometryGenerator::MeshData mesh;
	CGeometryGenerator::GetInstance()->CreateBox(width, height, depth, mesh);
	m_nVertices = mesh.Vertices.size();
	m_nIndices = mesh.Indices.size();

	vector<UINT>     indices(m_nIndices);
	vector<XMFLOAT3> pos(m_nVertices);
	vector<XMFLOAT3> normal(m_nVertices);
	vector<XMFLOAT2> uv(m_nVertices);

	for (int i = 0; i<m_nVertices; ++i)
	{
		pos[i] = mesh.Vertices[i].Position;
		normal[i] = mesh.Vertices[i].Normal;
		uv[i] = mesh.Vertices[i].TexC;
	}
	for (int i = 0; i < m_nIndices; ++i)
	{
		indices[i] = mesh.Indices[i];
	}

	m_dxgiIndexFormat = DXGI_FORMAT_R32_UINT;
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pPositionBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3), m_nVertices, &pos[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pNormalBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3), m_nVertices, &normal[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pUVBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT2), m_nVertices, &uv[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pIndexBuffer = SHADER_MANAGER->CreateBuffer(sizeof(UINT), m_nIndices, &indices[0], D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);

	ID3D11Buffer* ppBuffers[3] = { m_pPositionBuffer, m_pNormalBuffer, m_pUVBuffer };
	UINT pBufferStirdes[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pBufferOffsets[3] = { 0, 0, 0 };

	AssembleToVertexBuffer(3, ppBuffers, pBufferStirdes, pBufferOffsets);
}

CLightTexturedCubeMesh::~CLightTexturedCubeMesh()
{
	Memory::Release(m_pNormalBuffer);
	Memory::Release(m_pUVBuffer);
}

CInstancedLightTexturedCubeMesh::CInstancedLightTexturedCubeMesh(const float width, const float height, const float depth) : CLightTexturedCubeMesh(width, height, depth)
{
	// Max Object 1024..
	m_pInstancedBuffer = SHADER_MANAGER->CreateBuffer(sizeof(InstanceWorldData), 1024, nullptr, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	ID3D11Buffer* ppBuffers[1] = { m_pInstancedBuffer};
	UINT pBufferStirdes[1] = { sizeof(InstanceWorldData) };
	UINT pBufferOffsets[1] = { 0 };
	AssembleToVertexBuffer(1, ppBuffers, pBufferStirdes, pBufferOffsets);
}

CInstancedLightTexturedCubeMesh::~CInstancedLightTexturedCubeMesh()
{
	Memory::Release(m_pInstancedBuffer);
}
