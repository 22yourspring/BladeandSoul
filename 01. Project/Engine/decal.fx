#ifndef _DECAL
#define _DECAL

#include "value.fx"
#include "func.fx"

struct VS_INPUT
{
	float3 vPos : POSITION;
	float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    
    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VS_OUTPUT
{
	float4 vPosition : SV_POSITION;
    
    float3 vViewPos : POSITION;
    float3 vViewTangent : TANGENT;
    float3 vViewBinormal : BINORMAL;
    float3 vViewNormal : NORMAL;
    
	float2 vUV : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 vDiffuse : SV_Target0; // Diffuse
    float4 vNormal : SV_Target1; // Normal
    float4 vPosition : SV_Target2; // Position
    float4 vSpecular : SV_Target3; // Specular 계수
};



[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
}

// ========================
// Decal Shader
// ========================
// mesh : Cube Mesh
// blendstate : One-One
// depthstencilstate : No Depth check, No Depth Write

// g_tex_0 : Decal Diffuse Texture
// g_tex_1 : Position Target
// g_tex_2 : Decal Normal Texture

// g_vec2_0 : RenderTarget Resolution
// ===================================

VS_OUTPUT VS_Decal(VS_INPUT _in)
{
	VS_OUTPUT output = (VS_OUTPUT) 0.f;

	output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV).xyz);
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV).xyz);
	output.vUV = _in.vUV;

	return output;
}

PS_OUTPUT PS_Decal(VS_OUTPUT _in) : SV_Target
{
	PS_OUTPUT output = (PS_OUTPUT) 0.f;

	// Screen 좌표
	float2 vFullUV = float2(_in.vPosition.x / g_vec2_0.x, _in.vPosition.y / g_vec2_0.y);

	float3 vViewPos = g_tex_1.Sample(g_sam_0, vFullUV).xyz;
	
	float4 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv);
	float4 vLocalPos = mul(vWorldPos, g_matWorldInv);

	if (vLocalPos.x <= -0.501f || vLocalPos.y <= -0.501f || vLocalPos.z <= -0.501f
		|| vLocalPos.x >= 0.501f || vLocalPos.y >= 0.501f || vLocalPos.z >= 0.501f)
		clip(-1);

	if (vViewPos.x == 0.f || vViewPos.y == 0.f || vViewPos.z == 0.f)
	{
		clip(-1);
	}

	// Sphere 내부에 들어오는 정점인지 확인한다.

	float2 vTexUV = float2(vLocalPos.x + 0.5f, -(vLocalPos.z - 0.5f));
    
    float3 vViewNormal = _in.vViewNormal;
   
    {
        float3 vTSNormal = g_tex_2.Sample(g_sam_0, vTexUV).xyz;
        vTSNormal.xyz = (vTSNormal.xyz - 0.5f) * 2.f;
        float3x3 matTBN = { _in.vViewTangent, _in.vViewBinormal, _in.vViewNormal };
        vViewNormal = normalize(mul(vTSNormal, matTBN));
    }

    output.vDiffuse = g_tex_0.Sample(g_sam_0, vTexUV);
    output.vNormal.xyz = vViewNormal;
    output.vPosition.xyz = _in.vViewPos;
    output.vSpecular = g_vSpec;

    
	return output;
}

// 데칼 Decal 여기 Normal 안되는 부분 있음
#endif