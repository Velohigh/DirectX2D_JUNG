#ifndef _VALUE
#define _VALUE

#include "struct.fx"

cbuffer TRANSFORM : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
    
    row_major matrix g_matWV;
    row_major matrix g_matWVP;
};

cbuffer MATERIAL : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;

    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    matrix g_mat_0;
    matrix g_mat_1;
    matrix g_mat_2;
    matrix g_mat_3;
    
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btex_6;
    int g_btex_7;
};

// 전역 상수 레지스터, 어디서든 필요할 수 있는 값들
cbuffer GLOBAL : register(b2)
{
    float2 g_Resolution;
    float g_DT;
    float g_AccTime;
    uint g_Light2DCount;
    uint g_Light3DCount;
    int2 g_globalpadding;
}

// 하나의 재질안에 여러장의 텍스쳐를 쓰는 경우도 있다.
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);
Texture2D g_tex_7 : register(t7);

// 구조화 버퍼, Tex레지스터에 구조체를 바인딩시킨다. t레지스터는 참조하는식으로 작동하기에 가능.
StructuredBuffer<tLightInfo> g_Light2DBuffer : register(t8);

// 이미지에서 색상을 가져오기 위한 샘플러
SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);


#endif