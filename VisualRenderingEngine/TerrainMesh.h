#pragma once
#include "NormalMesh.h"

class CTerrainMesh : public CNormalMesh
{
public:
	CTerrainMesh();
	~CTerrainMesh();
private :
	ID3D11Buffer* m_pBoundYBuffer;
	ID3D11Buffer* m_pUVBuffer;
public :
	void SetBoundYBuffer(ID3D11Buffer* b) { m_pBoundYBuffer = b; }
	void SetUVBuffer(ID3D11Buffer* b) { m_pUVBuffer = b; }
};

