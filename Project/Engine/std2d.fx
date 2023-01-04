#ifndef _STD2D
#define _STD2D
// 2D에서 일반적으로 사용할 셰이더.

#include "value.fx"

struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// ============================
// Std2DShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_tex_0              : Output Texture
// ============================
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
        
    return output;
}


float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
        
    if (g_btex_0)   // 이미지가 세팅 되었다면, 해당 이미지UV 좌표 컬러로 샘플링 한다.
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else            // 이미지가 세팅되지 않았다면 마젠타 색 출력
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    if (0.f == vOutColor.a) // 알파값이 0이면 discard, 픽셀을 날려버린다. 출력 X
        discard;
    
    return vOutColor;
}


// ======================================
// Std2DLightShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_tex_0              : Output Texture
// g_tex_1              : Nomal Texture
// ======================================

struct VS_Light_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_Light_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;
};



VS_Light_OUT VS_Std2DLight(VS_Light_IN _in)
{
    VS_Light_OUT output = (VS_Light_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_matWorld);
    
        
    return output;
}


float4 PS_Std2DLight(VS_Light_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
        
    if (0.f == vOutColor.a)
        discard;
    
    // Lighting 처리
    float3 vLightColor = (float3) 0.f;
    for (int i = 0; i < iLightCount; ++i)
    {
        if (arrInfo[i].LightType == 0)
        {
            vLightColor += arrInfo[i].Color.vDiffuse;
        }
        else if (arrInfo[i].LightType == 1)
        {
            float3 vLightWorldPos = float3(arrInfo[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_in.vWorldPos.xy, 0.f);

            float fDistance = abs(distance(vWorldPos, vLightWorldPos));
            float fPow = saturate(1.f - (fDistance / arrInfo[i].Radius));
        
            vLightColor += arrInfo[i].Color.vDiffuse * fPow;
        }
    }
    
    vOutColor.rgb *= vLightColor;
    
    return vOutColor;
}

#endif