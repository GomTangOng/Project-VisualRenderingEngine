SamplerState gBasicSampler : register(s0);
TextureCube gCubeMap : register(t3);

cbuffer cbViewProjectionMtx : register(b0)
{
	matrix view;
	matrix projection;
}

cbuffer cbWorldMtx : register(b1)
{
	matrix world;
}

struct VertexIn
{
	float3 PosL : POSITION;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

VertexOut VS_SKYBOX(VertexIn vin)
{
	VertexOut vout;

	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
	vout.PosH = mul(mul(mul(float4(vin.PosL, 1.0f), world), view), projection).xyww;

	// Use local vertex position as cubemap lookup vector.
	vout.PosL = vin.PosL;

	return vout;
}

float4 PS_SKYBOX(VertexOut pin) : SV_Target
{
	return gCubeMap.Sample(gBasicSampler, pin.PosL);
}
