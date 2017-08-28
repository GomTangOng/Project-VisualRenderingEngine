#pragma once

#include "Mesh.h"

class COffScreenMesh : public CMesh
{
public:
	COffScreenMesh() = delete;
	COffScreenMesh(const float width, const float height);
	~COffScreenMesh();
private :
	ID3D11Buffer* m_pTexBuffer;
};

