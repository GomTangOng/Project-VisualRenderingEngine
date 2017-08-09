#pragma once

#include "stdafx.h"

class CLight
{
public:
	CLight();
	~CLight();

	XMFLOAT4 m_ambient;
	XMFLOAT4 m_diffuse;
	XMFLOAT4 m_speular;
};

class CDirectionLight : public CLight
{
public :
	CDirectionLight();
	~CDirectionLight();

	XMFLOAT3 m_direction;
	float    m_pad;
};

class CPointLight : public CLight
{
public :
	CPointLight();
	~CPointLight();

	XMFLOAT3 m_position;
	float    m_range;

	XMFLOAT3 m_attenuation;	// °¨¼â »ó¼ö
	float    m_pad;
};

class CSpotLight : public CLight
{
public :
	CSpotLight();
	~CSpotLight();

	XMFLOAT3 m_position;
	float    m_range;

	XMFLOAT3 m_direction;
	float    m_spot;

	XMFLOAT3 m_attenuation;
	float    m_pad;
};