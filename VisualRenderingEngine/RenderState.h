#pragma once

#include "Singleton.h"

#define RENDER_STATE CRenderState::GetInstance()

class CRenderState : public Singleton<CRenderState>
{
public:
	enum RASTER_STATE : UCHAR
	{
		RASTER_DEFAULT,
		RASTER_CULL_NONE,
		RASTER_END
	};

	enum DEPTH_STENCIL_STATE : UCHAR
	{
		DEPTH_STENCIL_DEFAULT,
		DEPTH_STENCIL_SKYBOX,
		DEPTH_STENCIL_END
	};

	CRenderState();
	~CRenderState();

	void Initalize();
	void Shutdown();

	void ClearRasterState();
	void ClearDepthStencilState();
	void SetRasterState(const UCHAR raster_type);
	void SetDepthStencilState(const UCHAR ds_type, const UINT stencil_cnt);

	ID3D11RasterizerState* GetRasterizerState(const UCHAR raster_type);
	ID3D11DepthStencilState* GetDepthStencilState(const UCHAR depth_stencil_type);
private:
	map<UCHAR, ID3D11RasterizerState*>   m_mapRasterizerState;
	map<UCHAR, ID3D11DepthStencilState*> m_mapDepthStencilState;
};

