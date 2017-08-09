#pragma once

#include "Mesh.h"

class CDiffusedMesh : public CMesh
{
public:
	CDiffusedMesh();
	virtual ~CDiffusedMesh();
protected :
	ID3D11Buffer* m_pColorBuffer;
public:
	void SetColorBuffer(ID3D11Buffer* pBuffer) { m_pColorBuffer = pBuffer; }
};

