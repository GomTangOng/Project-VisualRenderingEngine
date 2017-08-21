#include "stdafx.h"
#include "RenderState.h"
#include "VREngine.h"

CRenderState::CRenderState()
{
}


CRenderState::~CRenderState()
{
}

void CRenderState::Initalize()
{
	ID3D11RasterizerState *pRS;
	ID3D11DepthStencilState *pDS;
	D3D11_RASTERIZER_DESC rd;
	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(dsd));
	ZeroMemory(&rd, sizeof(rd));

	// SOILD & CULL_BACK
	rd.FillMode = D3D11_FILL_SOLID;	
	rd.CullMode = D3D11_CULL_BACK;
	HR(VR_ENGINE->GetDevice()->CreateRasterizerState(&rd, &pRS));
	m_mapRasterizerState[RASTER_STATE::RASTER_DEFAULT] = pRS;
	//m_vpRasterizerState.push_back(pRS);
	// SOILD & CULL_NONE
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	HR(VR_ENGINE->GetDevice()->CreateRasterizerState(&rd, &pRS));
	m_mapRasterizerState[RASTER_STATE::RASTER_CULL_NONE] = pRS;

	// DEFAULT
	dsd.DepthEnable = true;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	HR(VR_ENGINE->GetDevice()->CreateDepthStencilState(&dsd, &pDS));
	m_mapDepthStencilState[DEPTH_STENCIL_STATE::DEPTH_STENCIL_DEFAULT] = pDS;
	// 
	dsd.DepthEnable = true;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	HR(VR_ENGINE->GetDevice()->CreateDepthStencilState(&dsd, &pDS));
	m_mapDepthStencilState[DEPTH_STENCIL_STATE::DEPTH_STENCIL_SKYBOX] = pDS;
}

void CRenderState::Shutdown()
{
	for (auto& r : m_mapRasterizerState)
		Memory::Release(r.second);
	for (auto& ds : m_mapDepthStencilState)
		Memory::Release(ds.second);
}

void CRenderState::ClearRasterState()
{
	VR_ENGINE->GetDeviceContext()->RSSetState(0);
}

void CRenderState::ClearDepthStencilState()
{
	VR_ENGINE->GetDeviceContext()->OMSetDepthStencilState(0, 0);
}

void CRenderState::SetRasterState(const UCHAR raster_type)
{
	VR_ENGINE->GetDeviceContext()->RSSetState(GetRasterizerState(raster_type));
}

void CRenderState::SetDepthStencilState(const UCHAR ds_type, const UINT stencil_cnt)
{
	VR_ENGINE->GetDeviceContext()->OMSetDepthStencilState(GetDepthStencilState(ds_type), stencil_cnt);
}

ID3D11RasterizerState * CRenderState::GetRasterizerState(const UCHAR raster_type)
{
	return m_mapRasterizerState[raster_type];
}

ID3D11DepthStencilState * CRenderState::GetDepthStencilState(const UCHAR depth_stencil_type)
{
	return m_mapDepthStencilState[depth_stencil_type];
}
