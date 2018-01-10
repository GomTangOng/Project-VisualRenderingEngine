#include "stdafx.h"
#include "TerrainMesh.h"


CTerrainMesh::CTerrainMesh()
{
}


CTerrainMesh::~CTerrainMesh()
{
	Memory::Release(m_pUVBuffer);
}
