#pragma once

#include "DiffusedMesh.h"

class CTriangleMesh : public CDiffusedMesh
{
public:
	CTriangleMesh();
	~CTriangleMesh();

	void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext) override;
	void Render(ID3D11DeviceContext *pd3dDeviceContext) override;
	void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0) override;
};

