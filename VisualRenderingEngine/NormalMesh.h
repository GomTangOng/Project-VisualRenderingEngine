#pragma once
#include "Mesh.h"

class CNormalMesh : public CMesh
{
public:
	CNormalMesh();
	~CNormalMesh();
private :
	ID3D11Buffer *m_pNormalBuffer;
public:
	void SetNoramlConstantBuffer(ID3D11Buffer* b) { m_pNormalBuffer = b; }
};

