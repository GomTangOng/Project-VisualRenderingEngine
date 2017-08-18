#include "stdafx.h"
#include "TerrainManager.h"
#include "GeometryGenerator.h"
#include "TerrainMesh.h"
#include "ShaderManager.h"
#include "Utility.h"
#include "TerrainEntity.h"
#include "TextureManager.h"
#include "Texture.h"
#include "SlotType.h"
#include "TSTerrainMesh.h"
#include "SlotType.h"
CTerrainManager::CTerrainManager()
{
}


CTerrainManager::~CTerrainManager()
{
	//Memory::Release(m_pHeightMapSRV);
}

bool CTerrainManager::Initalize(ID3D11Device * pDevice)
{
	m_pDevice = pDevice;

	//BuildTerrain(320, 320, 50, 50);  No HeightMap Version.
	//LoadHeightMap(pTerrain);

	CTerrainEntity *pTerrain = new CTerrainEntity();
	CTSTerrainMesh *pTSTerrainMesh = new CTSTerrainMesh();
	pTerrain->Initalize();
	pTerrain->AddMesh(pTSTerrainMesh);
	pTerrain->SetTexture(TEXTURE_MANAGER->GetTexture(L"Grass"));

	UINT patchVertexRows = ((pTerrain->GetHeightMapHeight() - 1) / s_nCellsPerPatch) + 1;
	UINT patchVertexCols = ((pTerrain->GetHeightMapWidth() - 1) / s_nCellsPerPatch) + 1;
	pTerrain->SetPatchVertexRows(patchVertexRows);
	pTerrain->SetPatchVertexCols(patchVertexCols);
	pTerrain->SetPatchVertices(patchVertexRows * patchVertexCols);
	pTerrain->SetPatchQuadFaces((patchVertexRows - 1) * (patchVertexCols - 1));
	m_pTerrainEntity = pTerrain;

	LoadHeightMap(pTerrain);
	Smooth();
	CalcAllPatchBoundsY();
	BuildQuadPatchVB();
	BuildQuadPatchIB();
	BuildHeightmapSRV();

	return true;
}

void CTerrainManager::ShutDown()
{
	Memory::Delete(m_pTerrainEntity);
}

void CTerrainManager::BuildQuadPatchVB()
{
	float width        = m_pTerrainEntity->GetWidth();
	float depth        = m_pTerrainEntity->GetHeight();
	UINT patchVerCols = m_pTerrainEntity->GetPatchVertexCols();
	UINT patchVerRows = m_pTerrainEntity->GetPatchVertexRows();
	
	std::vector<XMFLOAT3> patchVertices_pos(patchVerCols * patchVerRows);
	std::vector<XMFLOAT2> patchVertices_tex0(patchVerCols * patchVerRows);
	std::vector<XMFLOAT2> patchVertices_boundY(patchVerCols * patchVerRows);

	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float patchWidth = width / (patchVerCols - 1);
	float patchDepth = depth / (patchVerRows - 1);
	float du = 1.0f / (patchVerCols - 1);
	float dv = 1.0f / (patchVerRows - 1);

	// Calculate Position and TextureUV
	for (UINT i = 0; i < patchVerRows; ++i)
	{
		float z = halfDepth - i*patchDepth;
		for (UINT j = 0; j < patchVerCols; ++j)
		{
			float x = -halfWidth + j*patchWidth;

			patchVertices_pos[i*patchVerCols + j] = XMFLOAT3(x, 0.0f, z);

			// Stretch texture over grid.
			patchVertices_tex0[i*patchVerCols + j].x = j*du;
			patchVertices_tex0[i*patchVerCols + j].y = i*dv;
		}
	}

	// Calculate Bound-Y
	// Store axis-aligned bounding box y-bounds in upper-left patch corner.
	for (UINT i = 0; i < patchVerRows - 1; ++i)
	{
		for (UINT j = 0; j < patchVerCols - 1; ++j)
		{
			UINT patchID = i*(patchVerCols - 1) + j;
			patchVertices_boundY[i*patchVerCols + j] = m_vPatchBoundsY[patchID];
		}
	}
	ID3D11Buffer* pQuadPatchVB[3];
	UINT stride[3]{ sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT2) };
	UINT offset[3]{ 0, 0, 0 };
	pQuadPatchVB[0] = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3),
												patchVertices_pos.size(),
												&patchVertices_pos[0],
												D3D11_BIND_VERTEX_BUFFER, 
												D3D11_USAGE_IMMUTABLE, 
												0);
	pQuadPatchVB[1] = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT2),
													patchVertices_tex0.size(),
													&patchVertices_tex0[0],
													D3D11_BIND_VERTEX_BUFFER,
													D3D11_USAGE_IMMUTABLE,
													0);
	pQuadPatchVB[2] = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT2),
													patchVertices_boundY.size(),
													&patchVertices_boundY[0],
													D3D11_BIND_VERTEX_BUFFER,
													D3D11_USAGE_IMMUTABLE,
													0);
	CMesh *pMesh = m_pTerrainEntity->GetMesh();
	pMesh->AssembleToVertexBuffer(3, pQuadPatchVB, stride, offset);
	pMesh->SetVertexCount(patchVertices_pos.size());
	pMesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
}

void CTerrainManager::BuildQuadPatchIB()
{
	std::vector<USHORT> indices(m_pTerrainEntity->GetPatchQuadFaces() * 4); // 4 indices per quad face

														 // Iterate over each quad and compute indices.
	int k = 0;
	UINT cols = m_pTerrainEntity->GetPatchVertexCols();
	UINT rows = m_pTerrainEntity->GetPatchVertexRows();
	for (UINT i = 0; i <rows - 1; ++i)
	{
		for (UINT j = 0; j <cols - 1; ++j)
		{
			// Top row of 2x2 quad patch
			indices[k] = i*cols + j;
			indices[k + 1] = i*cols + j + 1;

			// Bottom row of 2x2 quad patch
			indices[k + 2] = (i + 1)*cols + j;
			indices[k + 3] = (i + 1)*cols + j + 1;

			k += 4; // next quad
		}
	}

	ID3D11Buffer* pIndexBuffer;
	pIndexBuffer = SHADER_MANAGER->CreateBuffer(sizeof(USHORT), indices.size(), &indices[0], D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
	//D3D11_BUFFER_DESC ibd;
	//ibd.Usage = D3D11_USAGE_IMMUTABLE;
	//ibd.ByteWidth = sizeof(USHORT) * indices.size();
	//ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//ibd.CPUAccessFlags = 0;
	//ibd.MiscFlags = 0;
	//ibd.StructureByteStride = 0;

	//D3D11_SUBRESOURCE_DATA iinitData;
	//iinitData.pSysMem = &indices[0];
	//HR(device->CreateBuffer(&ibd, &iinitData, &mQuadPatchIB));
	CMesh *pMesh = m_pTerrainEntity->GetMesh();
	pMesh->SetIndexBuffer(pIndexBuffer);
	pMesh->SetIndexCount(indices.size());
	pMesh->SetDXGIIndexFormat(DXGI_FORMAT_R16_UINT);
}

void CTerrainManager::BuildHeightmapSRV()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_pTerrainEntity->GetWidth();
	texDesc.Height = m_pTerrainEntity->GetHeight();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;		
	texDesc.MiscFlags = 0;
	
	vector<HALF> hmap(m_vHeightMap.size());
	transform(m_vHeightMap.begin(), m_vHeightMap.end(), hmap.begin(), XMConvertFloatToHalf);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem          = &hmap[0];
	data.SysMemPitch      = m_pTerrainEntity->GetHeightMapWidth() * sizeof(HALF);
	data.SysMemSlicePitch = 0;

	ID3D11Texture2D* hmapTex = 0;
	HR(m_pDevice->CreateTexture2D(&texDesc, &data, &hmapTex));

	m_pHeightMap = new CTexture();

	ID3D11ShaderResourceView *pSRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	HR(m_pDevice->CreateShaderResourceView(hmapTex, &srvDesc, &pSRV));
	
	m_pHeightMap->SetSRV(pSRV);
	m_pHeightMap->SetTextureSlot(TextureSlot::TEXTURE_HEIGHTMAP);
	m_pHeightMap->SetSampler(TEXTURE_MANAGER->GetSampler(L"HeightmapSampler"));
	m_pHeightMap->SetSamplerSlot(SamplerSlot::SAMPLER_HEIGHTMAP);
	TEXTURE_MANAGER->AddTexture(L"HeightMap", m_pHeightMap);
	// SRV saves reference.
	Memory::Release(hmapTex);
}

bool CTerrainManager::LoadHeightMap(CTerrainEntity *pTerrain)
{
	UINT height_cnt = pTerrain->GetHeightMapWidth() * pTerrain->GetHeightMapHeight();
	// A height for each vertex
	std::vector<unsigned char> in(height_cnt);

	// Open the file.
	std::ifstream inFile;
	inFile.open(pTerrain->GetHeightMapFileName().c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}
	else
	{
		return false;
	}

	// Copy the array data into a float array and scale it.
	m_vHeightMap.resize(height_cnt, 0);
	for (UINT i = 0; i < height_cnt; ++i)
	{
		m_vHeightMap[i] = (in[i] / 255.0f) * pTerrain->GetHeightScale();
	}
	return true;
}

void CTerrainManager::BuildTerrain(const float width, const float depth, UINT m, UINT n)
{
	//CTerrainMesh* pMesh = new CTerrainMesh();

	//m_vMeshes.push_back(pMesh);
	//
	//CGeometryGenerator::MeshData grid;
	//CGeometryGenerator geoGen;
	//geoGen.CreateGrid(width, depth, m, n, grid);
	//UINT vertices_cnt = grid.Vertices.size();
	//UINT indices_cnt = grid.Indices.size();

	//m_vMeshes.reserve(vertices_cnt);
	//
	//vector<XMFLOAT3> positions(vertices_cnt);
	//vector<XMFLOAT3> normals(vertices_cnt);
	//for (int i=0; i<vertices_cnt; ++i)
	//{
	//	XMFLOAT3 p = grid.Vertices[i].Position;
	//	p.y = GetHeight(p.x, p.z);
	//	positions[i] = p;
	//	normals[i] = GetHillNormal(p.x, p.z);
	//	//if (p.y < -10.0f)
	//	//{
	//	//	// Sandy beach color.
	//	//	colors[i] = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
	//	//}
	//	//else if (p.y < 5.0f)
	//	//{
	//	//	// Light yellow-green.
	//	//	colors[i] = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	//	//}
	//	//else if (p.y < 12.0f)
	//	//{
	//	//	// Dark yellow-green.
	//	//	colors[i] = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
	//	//}
	//	//else if (p.y < 20.0f)
	//	//{
	//	//	// Dark brown.
	//	//	colors[i] = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
	//	//}
	//	//else
	//	//{
	//	//	// White snow.
	//	//	colors[i] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	//}
	//}

	//ID3D11Buffer* pPositionBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT3), vertices_cnt, &positions[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
	//ID3D11Buffer* pNormalsBuffer = SHADER_MANAGER->CreateBuffer(sizeof(XMFLOAT4), vertices_cnt, &normals[0], D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
	//ID3D11Buffer* pIndicesBuffer = SHADER_MANAGER->CreateBuffer(sizeof(UINT), indices_cnt, &grid.Indices[0], D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_IMMUTABLE, 0);
	//ID3D11Buffer* pBuffers[2]{ pPositionBuffer, pNormalsBuffer };
	//UINT pBufferStrides[2]{ sizeof(XMFLOAT3), sizeof(XMFLOAT3) };
	//UINT pBufferOffset[2]{ 0, 0 };

	//pMesh->SetVertexCount(vertices_cnt);
	//pMesh->SetIndexCount(indices_cnt);
	//pMesh->SetDXGIIndexFormat(DXGI_FORMAT_R32_UINT);
	//pMesh->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pMesh->SetPositionBuffer(pPositionBuffer);
	//pMesh->SetIndexBuffer(pIndicesBuffer);
	//pMesh->SetNoramlConstantBuffer(pNormalsBuffer);
	////pMesh->SetColorBuffer(pColorsBuffer);
	//pMesh->AssembleToVertexBuffer(2, pBuffers, pBufferStrides, pBufferOffset);
}

float CTerrainManager::GetHeight(const float x, const float z)
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*m_pTerrainEntity->GetWidth()) / m_pTerrainEntity->GetCellSpacing();
	float d = (z - 0.5f*m_pTerrainEntity->GetHeight()) / -m_pTerrainEntity->GetCellSpacing();

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_vHeightMap[row*m_pTerrainEntity->GetHeightMapWidth() + col];
	float B = m_vHeightMap[row*m_pTerrainEntity->GetHeightMapWidth() + col + 1];
	float C = m_vHeightMap[(row + 1)*m_pTerrainEntity->GetHeightMapWidth() + col];
	float D = m_vHeightMap[(row + 1)*m_pTerrainEntity->GetHeightMapWidth() + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s)*uy + (1.0f - t)*vy;
	}
}

void CTerrainManager::Smooth()
{
	std::vector<float> dest(m_vHeightMap.size());

	for (UINT i = 0; i < m_pTerrainEntity->GetHeightMapHeight(); ++i)
	{
		for (UINT j = 0; j < m_pTerrainEntity->GetHeightMapWidth(); ++j)
		{
			dest[i * m_pTerrainEntity->GetHeightMapWidth() + j] = Average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_vHeightMap = dest;
}

void CTerrainManager::CalcAllPatchBoundsY()
{
	m_vPatchBoundsY.resize(m_pTerrainEntity->GetPatchQuadFaces());

	// For each patch
	for (UINT i = 0; i < m_pTerrainEntity->GetPatchVertexRows() - 1; ++i)
	{
		for (UINT j = 0; j < m_pTerrainEntity->GetPatchVertexCols() - 1; ++j)
		{
			CalcPatchBoundsY(i, j);
		}
	}
}

void CTerrainManager::CalcPatchBoundsY(const UINT i, const UINT j)
{
	UINT x0 = j*s_nCellsPerPatch;
	UINT x1 = (j + 1)*s_nCellsPerPatch;

	UINT y0 = i*s_nCellsPerPatch;
	UINT y1 = (i + 1)*s_nCellsPerPatch;
	
	float minY = +FLT_MAX;
	float maxY = -FLT_MAX;
	for (UINT y = y0; y <= y1; ++y)
	{
		for (UINT x = x0; x <= x1; ++x)
		{
			UINT k = y * m_pTerrainEntity->GetHeightMapWidth() + x;
			minY = min(minY, m_vHeightMap[k]);
			maxY = max(maxY, m_vHeightMap[k]);
		}
	}

	UINT patchID = i*(m_pTerrainEntity->GetPatchVertexCols() - 1) + j;
	m_vPatchBoundsY[patchID] = XMFLOAT2(minY, maxY);
}

float CTerrainManager::Average(const int x, const int z)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	// Use int to allow negatives.  If we use UINT, @ i=0, m=i-1=UINT_MAX
	// and no iterations of the outer for loop occur.
	for (int m = z - 1; m <= z + 1; ++m)
	{
		for (int n = x - 1; n <= x + 1; ++n)
		{
			if (InBounds(n, m))
			{
				avg += m_vHeightMap[m * m_pTerrainEntity->GetHeightMapWidth() + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

bool CTerrainManager::InBounds(const int x, const int z)
{
	// True if xz are valid indices; false otherwise.
	return z >= 0 && z < (int)m_pTerrainEntity->GetHeight() && x >= 0 && x < (int)m_pTerrainEntity->GetWidth();
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