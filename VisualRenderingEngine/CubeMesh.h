#pragma once

#include "DiffusedMesh.h"

class CCubeMesh : public CMesh
{
public:
	CCubeMesh() = delete;
	CCubeMesh(const float width, const float height, const float depth);
	~CCubeMesh();
};

class CLightCubeMesh : public CMesh
{
public:
	CLightCubeMesh() = delete;
	CLightCubeMesh(const float width, const float height, const float depth);
	~CLightCubeMesh();
private:
	ID3D11Buffer* m_pNormalBuffer;
};

class CLightTexturedCubeMesh : public CMesh
{
public:
	CLightTexturedCubeMesh() = delete;
	CLightTexturedCubeMesh(const float width, const float height, const float depth);
	~CLightTexturedCubeMesh();
private:
	ID3D11Buffer* m_pNormalBuffer;
	ID3D11Buffer* m_pUVBuffer;
};

class CDiffusedCubeMesh : public CDiffusedMesh
{
public:
	CDiffusedCubeMesh();
	//CCubeMesh(const float width, const float height, const float depth);
	~CDiffusedCubeMesh();


	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);
};

