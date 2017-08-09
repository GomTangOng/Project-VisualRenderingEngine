#include "stdafx.h"
#include "Light.h"


CLight::CLight()
{
	ZeroMemory(this, sizeof(this));
}


CLight::~CLight()
{
}

CDirectionLight::CDirectionLight()
{
	ZeroMemory(this, sizeof(this));
}

CDirectionLight::~CDirectionLight()
{
}

CPointLight::CPointLight()
{
	ZeroMemory(this, sizeof(this));
}

CPointLight::~CPointLight()
{
}

CSpotLight::CSpotLight()
{
	ZeroMemory(this, sizeof(this));
}

CSpotLight::~CSpotLight()
{
}
