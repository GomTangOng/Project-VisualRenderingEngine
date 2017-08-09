#include "stdafx.h"
#include "TriangleMesh.h"
#include "DiffusedMesh.h"

CTriangleMesh::CTriangleMesh()
{
	
}


CTriangleMesh::~CTriangleMesh()
{
	CDiffusedMesh::~CDiffusedMesh();
}

void CTriangleMesh::OnPrepareRender(ID3D11DeviceContext * pd3dDeviceContext)
{
	CMesh::OnPrepareRender(pd3dDeviceContext);
}

void CTriangleMesh::Render(ID3D11DeviceContext * pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}

void CTriangleMesh::RenderInstanced(ID3D11DeviceContext * pd3dDeviceContext, int nInstances, int nStartInstance)
{
	CMesh::RenderInstanced(pd3dDeviceContext, nInstances, nStartInstance);
}
