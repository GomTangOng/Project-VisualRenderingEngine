#include "stdafx.h"
#include "SkyBoxEntity.h"
#include "Mesh.h"
#include "GeometryGenerator.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "VREngine.h"
CSkyBoxEntity::CSkyBoxEntity()
{
}


CSkyBoxEntity::~CSkyBoxEntity()
{
}

void CSkyBoxEntity::Initalize()
{
	// Generate Mesh
	CGeometryGenerator::MeshData sphere;
	CGeometryGenerator geoGen;
	geoGen.CreateSphere(5000.0f, 30, 30, sphere);
	
	CMesh *pMesh = new CMesh();
	std::vector<XMFLOAT3> vertices(sphere.Vertices.size());

	// Set Position
	for (size_t i = 0; i < sphere.Vertices.size(); ++i)
	{
		vertices[i] = sphere.Vertices[i].Position;
	}
	//pMesh->SetPositionBuffer();
	ID3D11Buffer* pPosBuf = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3), sphere.Vertices.size(), &vertices[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	pMesh->SetDXGIIndexFormat(DXGI_FORMAT_R16_UINT);
	pMesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pMesh->SetPositionBuffer(pPosBuf);
	pMesh->AssembleToVertexBuffer(1, &pPosBuf, &stride, &offset);
	// Set Index
	pMesh->SetIndexCount(sphere.Indices.size());
	std::vector<USHORT> indices16;
	indices16.assign(sphere.Indices.begin(), sphere.Indices.end());
	ID3D11Buffer* pIndexBuf = SHADER_MANAGER->CreateBuffer(sizeof(USHORT), sphere.Indices.size(), &indices16[0], D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	pMesh->SetIndexBuffer(pIndexBuf);

	AddMesh(pMesh);
	// Set Texture
	m_pTexture = TEXTURE_MANAGER->GetTexture(L"Grass_SkyBox");
}

void CSkyBoxEntity::Update(const float fTimeElapsed)
{
	CEntity::Update(fTimeElapsed);
}

void CSkyBoxEntity::OnPrepareRender()
{
	CEntity::OnPrepareRender();
}

void CSkyBoxEntity::Render()
{
	CEntity::Render();
}
