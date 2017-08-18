#pragma once
#include "NormalMesh.h"

class CTerrainMesh : public CNormalMesh
{
public:
	CTerrainMesh();
	~CTerrainMesh();
private :
	ID3D11Buffer* m_pBoundYBuffer;
public :
	void SetBoundYBuffer(ID3D11Buffer* b) { m_pBoundYBuffer = b; }
};

