#ifndef _TEST
#define _TEST

// 상수버퍼 레지스터
cbuffer TRANSFORM : register(b0)
{
    float4 vPlayerPos;
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
};

// 하나의 재질안에 여러장의 텍스쳐를 쓰는 경우도 있다.
Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);
Texture2D g_tex_7 : register(t7);


// 이미지에서 색상을 가져오기 위한 샘플러
SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);






// VS 입력 구조체
struct VS_IN
{
	float3 vPos     : POSITION;
	float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD;
};

// 반환 구조체
struct VS_OUT
{
    float4 vPosition    : SV_Position;
    float4 vOutColor    : COLOR;
    float2 vOutUV       : TEXCOORD;
};

// vertex shader
// LocalSpace 물체를 -> 월드 스페이스 -> 카메라 -> NDC 좌표계로 이동
VS_OUT VS_Test(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 입력으로 들어온 정점좌표에 상수버퍼 값을 더해서 출력 ( 월드 스페이스 변환 )
    float3 vPos = _in.vPos;     
    vPos.xy += vPlayerPos.xy;   
    
    output.vPosition = float4(vPos, 1.f);
    output.vOutColor = _in.vColor;
    output.vOutUV = _in.vUV;
    
    return output;
}

// pixel shader
float4 PS_Test(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if(g_int_0 == 0)
        vColor = g_tex_0.Sample(g_sam_0, _in.vOutUV);
    else if(g_int_0 == 1)
        vColor = g_tex_0.Sample(g_sam_1, _in.vOutUV);
    
    return vColor;
}
#endif