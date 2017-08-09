#pragma once

#include "stdafx.h"

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct TextureVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Tex;
};

struct LightVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
};

struct LightTextureVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
};

// Name : Vertex 
// Desc : GeomatryGenerator에서 일반적인 메시 데이터 집어 넣을 버텍스 구조체
//
struct Vertex
{
	Vertex() {}
	Vertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv)
		: Position(p), Normal(n), TangentU(t), TexC(uv) {}
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v)
		: Position(px, py, pz), Normal(nx, ny, nz),
		TangentU(tx, ty, tz), TexC(u, v) {}

	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT3 TangentU;
	XMFLOAT2 TexC;
};