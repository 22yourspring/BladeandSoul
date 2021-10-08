#ifndef _COMPUTE
#define _COMPUTE

#include "value.fx"
#include "func.fx"


// Dispatch(1, 1024, 1)
[numthreads(1024, 1, 1)] // 1024 -> Group
void COMPUTE_TEST(int3 _iThreadIdx : SV_DispatchThreadID)
{
    g_rwtex_0[_iThreadIdx.xy] = float4(0.f, 0.f, 0.f, 1.f);
    if (_iThreadIdx.y == 512)
        g_rwtex_0[_iThreadIdx.xy] = float4(1.f, 0.f, 0.f, 1.f);
}


// ==========================
// Edit Height Compute Shader
// g_rwtex_0 : Height Map
// g_arrtex_0 : Brush Array Texture
// g_int_0 : Brush Index

// g_vec2_0 : Brush Position
// g_vec2_1 : Brush Size
// g_vec2_2 : HeightMap Resolution
// ==========================
[numthreads(1024, 1, 1)] // 1024
void CS_HeightMap(uint3 _iThreadIdx : SV_DispatchThreadID)
{
    uint2 CenterPos = uint2((uint) (g_vec2_2.x * g_vec2_0.x), (uint) (g_vec2_2.y * g_vec2_0.y));
    uint2 BrushSize = uint2((uint) (g_vec2_2.x * g_vec2_1.x), (uint) (g_vec2_2.y * g_vec2_1.y));
    
    if (CenterPos.x - (BrushSize.x / 2) < _iThreadIdx.x && _iThreadIdx.x < CenterPos.x + (BrushSize.x / 2)
        && CenterPos.y - (BrushSize.y / 2) < _iThreadIdx.y && _iThreadIdx.y < CenterPos.y + (BrushSize.y / 2))
    {        
        if(texarr_0)
        {
            float2 vThreadPos = (float2) _iThreadIdx.xy / g_vec2_2;  // Thread 위치를 높이맵 기준 비율로 전환       
            float2 vLTPos = g_vec2_0 - (g_vec2_1 / 2.f); // Brush Position - (Brush Size / 2), 좌상단 위치        
            float2 vBrushUV = (vThreadPos - vLTPos) / g_vec2_1;   // 스레드 대상 픽셀이 브러쉬에서 참조해야하는 UV 값
            g_rwtex_0[_iThreadIdx.xy] += g_arrtex_0.SampleLevel(g_sam_0, float3(vBrushUV, g_int_0), 0).a * g_fDT;
        }
        else
        {
            float fDist = distance((float2) CenterPos, (float2) _iThreadIdx.xy);
            float fAdd = cos((fDist / ((float) BrushSize.x / 2.f)) * (3.1415926535f / 2.f)) * g_fDT;
            if (fAdd > 0.f)
                g_rwtex_0[_iThreadIdx.xy] += fAdd;
        }
    }
    
    if (g_vec2_2.x == 0.f)
        g_rwtex_0[_iThreadIdx.xy] += g_fDT * 1.f;
}


// =======================
// CS_Weight
// g_rwtex_0 : 출력 텍스쳐 (weight texture)
// g_arrtex_0 : brush Texture

// g_int_2 : Tile Index (가중치를 올릴 타일)
// g_int_3 : Brush Index

// g_vec2_0 : 브러쉬 중심 위치(높이맵 좌상단 기준)
// g_vec2_1 : 브러쉬 크기
// g_vec2_2 : 가중치 해상도
// =======================

[numthreads(1024, 1, 1)] // 1024
void CS_Weight(int3 _iThreadID : SV_DispatchThreadID)
{
    float2 vLTPos = g_vec2_0 - (g_vec2_1 / 2.f); // Brush 좌상단 위치    
    float2 vThreadPos = float2((float) _iThreadID.x / (float) g_vec2_2.x, (float) _iThreadID.y / (float) g_vec2_2.y); // 스레드의 위치값
    
    float2 vBrushUV = (vThreadPos - vLTPos) / g_vec2_1; // 브러쉬 추출 UV 값
    
    if (0.f <= vBrushUV.x && vBrushUV.x <= 1.f
        && 0.f <= vBrushUV.y && vBrushUV.y <= 1.f)
    {
        float fRatio = saturate(cos((distance(g_vec2_0, vThreadPos) / (g_vec2_1.x / 2.f)) * (3.141592 / 2.f)));
        
        float vAlpha = g_arrtex_0.SampleLevel(g_sam_0, float3(vBrushUV, g_int_3), 0).a;
        if (vAlpha != 0.f)
        {
            float fAdd = g_fDT * fRatio * vAlpha * 4.f;
            float4 vOrigin = g_rwtex_0[_iThreadID.xy];
            if (g_int_2 == 0)
            {
                vOrigin[0] += fAdd;
                if (vOrigin[0] > 1.f)
                    vOrigin[0] = 1.f;
                
                float fOver = vOrigin[0] + vOrigin[1] + vOrigin[2] + vOrigin[3] - 1.f;
                if (fOver < 0.f)
                    fOver = 0.f;
                
                float fRest = vOrigin[1] + vOrigin[2] + vOrigin[3];
                if (fRest == 0.f)
                    fRest = 1.f;
                
                vOrigin[1] -= fOver * (vOrigin[1] / fRest);
                vOrigin[2] -= fOver * (vOrigin[2] / fRest);
                vOrigin[3] -= fOver * (vOrigin[3] / fRest);
                
                g_rwtex_0[_iThreadID.xy] = vOrigin;
            }
            else if (g_int_2 == 1)
            {
                vOrigin[1] += fAdd;
                if (vOrigin[1] > 1.f)
                    vOrigin[1] = 1.f;
                
                float fOver = vOrigin[0] + vOrigin[1] + vOrigin[2] + vOrigin[3] - 1.f;
                if (fOver < 0.f)
                    fOver = 0.f;
                
                float fRest = vOrigin[0] + vOrigin[2] + vOrigin[3];
                if (fRest == 0.f)
                    fRest = 1.f;
                
                vOrigin[0] -= fOver * (vOrigin[0] / fRest);
                vOrigin[2] -= fOver * (vOrigin[2] / fRest);
                vOrigin[3] -= fOver * (vOrigin[3] / fRest);
                
                g_rwtex_0[_iThreadID.xy] = vOrigin;
            }
            else if (g_int_2 == 2)
            {
                vOrigin[2] += fAdd;
                if (vOrigin[2] > 1.f)
                    vOrigin[2] = 1.f;
                
                float fOver = vOrigin[0] + vOrigin[1] + vOrigin[2] + vOrigin[3] - 1.f;
                if (fOver < 0.f)
                    fOver = 0.f;
                
                float fRest = vOrigin[0] + vOrigin[1] + vOrigin[3];
                if (fRest == 0.f)
                    fRest = 1.f;
                
                vOrigin[0] -= fOver * (vOrigin[0] / fRest);
                vOrigin[1] -= fOver * (vOrigin[1] / fRest);
                vOrigin[3] -= fOver * (vOrigin[3] / fRest);
                
                g_rwtex_0[_iThreadID.xy] = vOrigin;
            }
            else if (g_int_2 == 3)
            {
                vOrigin[3] += fAdd;
                if (vOrigin[3] > 1.f)
                    vOrigin[3] = 1.f;
                
                float fOver = vOrigin[0] + vOrigin[1] + vOrigin[2] + vOrigin[3] - 1.f;
                if (fOver < 0.f)
                    fOver = 0.f;
                
                float fRest = vOrigin[0] + vOrigin[1] + vOrigin[2];
                if (fRest == 0.f)
                    fRest = 1.f;
                
                vOrigin[0] -= fOver * (vOrigin[0] / fRest);
                vOrigin[1] -= fOver * (vOrigin[1] / fRest);
                vOrigin[2] -= fOver * (vOrigin[2] / fRest);
                
                g_rwtex_0[_iThreadID.xy] = vOrigin;
            }
        }
    }
}

#endif