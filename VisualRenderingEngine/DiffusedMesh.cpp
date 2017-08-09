#include "stdafx.h"
#include "DiffusedMesh.h"
#include "Utility.h"
#include "ElementType.h"

CDiffusedMesh::CDiffusedMesh()
{
	m_nType |= VERTEX_COLOR_ELEMENT;
	m_pColorBuffer = nullptr;
}


CDiffusedMesh::~CDiffusedMesh()
{
	CMesh::~CMesh();
	Memory::Release(m_pColorBuffer);
}
