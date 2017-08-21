
#define MAX_LIGHT 16

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeDirectionalLight(Material mat, DirectionalLight L,
	float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -L.Direction;

	// Add ambient term.
	ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a point light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;

	// The distance from surface to light.
	float d = length(lightVec);

	// Range test.
	if (d > L.Range)
		return;

	// Normalize the light vector.
	lightVec /= d;

	// Ambient term.
	ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	// Attenuate
	float att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec *= att;
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;

	// The distance from surface to light.
	float d = length(lightVec);

	// Range test.
	if (d > L.Range)
		return;

	// Normalize the light vector.
	lightVec /= d;

	// Ambient term.
	ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	// Scale by spotlight factor and attenuate.
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

	// Scale by spotlight factor and attenuate.
	float att = spot / dot(L.Att, float3(1.0f, d, d*d));

	ambient *= spot;
	diffuse *= att;
	spec *= att;
}


//-------------------------------------
// @ Textures
//
Texture2D gTexture01 : register(t0);
TextureCube gCubeMap : register(t3);
//------------------------------------
// @ SamplerState
//
SamplerState gBasicSampler : register(s0);


//-----------------------------------
// @ Contant Buffer
//

cbuffer cbViewProjectionMtx : register(b0)
{
	matrix view;
	matrix projection;
}

cbuffer cbWorldMtx : register(b1)
{
	matrix world;
}

cbuffer cbLight : register(b0)
{
	DirectionalLight gDirLight[MAX_LIGHT];
	PointLight gPointLight[MAX_LIGHT];
	SpotLight gSpotLight[MAX_LIGHT];
	float3	 gCamPosW;
};

cbuffer cbMaterial : register(b1)
{
	Material gMaterial;
}

//--------------------------------------------------------------------------------------
// @ Structure
// 

struct VS_LIGHTING_INPUT
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
};

struct VS_LIGHTING_TEXTURE_INPUT
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD0;
};

struct VS_LIGHTING_TEXTURE_OUTPUT
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD0;
};

struct VS_LIGHTING_OUTPUT
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};
//struct VertexIn
//{
//	float3 PosL : POSITION;
//};
//
//struct VertexOut
//{
//	float4 PosH : SV_POSITION;
//	float3 PosL : POSITION;
//};
//------------------------------------------------------------
// @ Shader Function 
//
//RWTexture2D<float4> t1;
//RWTexture2D<float4> t2;
//
//float4 PS_INTERACE(VS_OUTPUT vo)
//{
//	for (i = 0; i < scw / 2; i++) {
//		float4 t = t1.Sample(gBasicSampler, vo.tex);
//		subRect.left = i; subRect.right = i + 1;
//		rightRect.left = offset + i * 2 + 1; rightRect.right = rightRect.left + 1;
//		t2[int2(vo.te)]
//	}
//}


//VertexOut VS_SKYBOX(VertexIn vin)
//{
//	VertexOut vout;
//
//	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
//	vout.PosH = mul(float4(vin.PosL, 1.0f), world).xyzw;
//	vout.PosH = mul(vout.PosH, view);
//	vout.PosH = mul(vout.PosH, projection);
//	//vout.PosH = mul(mul(mul(float4(vin.PosL, 1.0f), world), view), projection).xyww;
//
//	// Use local vertex position as cubemap lookup vector.
//	vout.PosL = vin.PosL;
//
//	return vout;
//}
//
//float4 PS_SKYBOX(VertexOut pin) : SV_Target
//{
//	//return float4(1.0f, 0.0f, 1.0f, 1.0f);
//	return gCubeMap.Sample(gBasicSampler, pin.PosL);
//}


float4 VS(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}

float4 PS(float4 pos : SV_POSITION) : SV_Target
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}

VS_OUTPUT VS2(float4 Pos : POSITION, float4 Color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, world);
	output.Pos = mul(output.Pos, view);
	output.Pos = mul(output.Pos, projection);
	output.Color = Color;
	return output;
}

float4 PS2(VS_OUTPUT input) : SV_Target
{
	return input.Color;
}

VS_LIGHTING_OUTPUT VS_LIGHTING(VS_LIGHTING_INPUT input)
{
	VS_LIGHTING_OUTPUT output = (VS_LIGHTING_OUTPUT)output;
	output.PosW = mul(float4(input.PosL, 1.0f), world).xyz;
	output.PosH = mul(mul(mul(float4(input.PosL, 1.0f), world), view), projection);
	output.NormalW = mul(input.NormalL, (float3x3)world);
	return output;
}

VS_LIGHTING_TEXTURE_OUTPUT VS_LIGHTING_TEXTURED(VS_LIGHTING_TEXTURE_INPUT input)
{
	VS_LIGHTING_TEXTURE_OUTPUT output = (VS_LIGHTING_TEXTURE_OUTPUT)output;
	output.PosW    = mul(float4(input.PosL, 1.0f), world).xyz;
	output.PosH    = mul(mul(mul(float4(input.PosL, 1.0f), world), view), projection);
	output.NormalW = mul(input.NormalL, (float3x3)world);
	output.Tex     = input.Tex;
	return output;
}

float4 PS_LIGHTING_TEXTURED(VS_LIGHTING_TEXTURE_OUTPUT input) : SV_TARGET
{
	input.NormalW = normalize(input.NormalW);
	float3 toEyeW = normalize(gCamPosW - input.PosW);
	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	float4 A, D, S;
	float4 texColor = float4(1, 1, 1, 1);
	texColor = gTexture01.Sample(gBasicSampler, input.Tex);
	float4 litColor = texColor;

	[unroll]
	for (int i = 0; i < MAX_LIGHT; ++i)
	{
		[flatten]
		if (gDirLight[i].pad == 1.0f)
		{
			ComputeDirectionalLight(gMaterial, gDirLight[i], input.NormalW, toEyeW, A, D, S);
			ambient += A;
			diffuse += D;
			spec += S;
		}

		[flatten]
		if (gPointLight[i].pad == 1.0f)
		{
			ComputePointLight(gMaterial, gPointLight[i], input.PosW, input.NormalW, gCamPosW, A, D, S);
			ambient += A;
			diffuse += D;
			spec += S;
		}

		[flatten]
		if (gSpotLight[i].pad == 1.0f)
		{
			ComputeSpotLight(gMaterial, gSpotLight[i], input.PosW, input.NormalW, gCamPosW, A, D, S);
			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	litColor = texColor * (ambient + diffuse) + spec;

	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

	return litColor;
}

float4 PS_LIGHTING(VS_LIGHTING_OUTPUT input) : SV_Target
{
	input.NormalW = normalize(input.NormalW);
	float3 toEyeW = normalize(gCamPosW - input.PosW);
	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	float4 A, D, S;

	[unroll]
	for (int i = 0; i < MAX_LIGHT; ++i)
	{
		[flatten]
		if (gDirLight[i].pad == 1.0f)
		{
			ComputeDirectionalLight(gMaterial, gDirLight[i], input.NormalW, toEyeW, A, D, S);
			ambient += A;
			diffuse += D;
			spec += S;
		}
		
		[flatten]
		if (gPointLight[i].pad == 1.0f)
		{
			ComputePointLight(gMaterial, gPointLight[i], input.PosW, input.NormalW, gCamPosW, A, D, S);
			ambient += A;
			diffuse += D;
			spec += S;
		}
		
		[flatten]
		if (gSpotLight[i].pad == 1.0f)
		{
			ComputeSpotLight(gMaterial, gSpotLight[i], input.PosW, input.NormalW, gCamPosW, A, D, S);
			ambient += A;
			diffuse += D;
			spec += S;
		}
	}
	

	float4 litColor = ambient + diffuse + spec;

	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.Diffuse.a;

	return litColor;
}

