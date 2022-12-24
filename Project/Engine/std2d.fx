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


#endif