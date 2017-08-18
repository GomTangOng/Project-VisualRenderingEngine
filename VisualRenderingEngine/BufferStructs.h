#pragma once
#include "stdafx.h"
#include "Material.h"
#include "Light.h"


// ConstantBuffer
struct ConstantBuffer
{
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProjection;
};

struct VS_CB_WORLD_MATRIX
{
	XMFLOAT4X4 mtxWorld;
};

struct VS_CB_VIEW_PROJECTION_MATRIX
{
	XMFLOAT4X4 mtxView;
	XMFLOAT4X4 mtxProjection;
};

struct PS_CB_LIGHT
{
	CDirectionLight dirLight[MAX_LIGHT];
	CPointLight     pointLight[MAX_LIGHT];
	CSpotLight      spotLight[MAX_LIGHT];
	XMFLOAT3        camPos;
	float pad;	
};

struct PS_CB_MATERIAL
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT4 reflect;
};

struct TS_CB_FACTOR 
{
	XMFLOAT4X4 viewProjection;
	XMFLOAT3   camPosW;
	float      minDist;

	float      maxDist;
	float      minTess;
	float      maxTess;
	float      texelCellSpaceU;

	float      texelCellSpaceV;
	float      worldCellSpace;
	XMFLOAT2   texScale;
	
	XMFLOAT4   worldFrustumPlanes[6];
};