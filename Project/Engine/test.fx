#ifndef _TEST
#define _TEST

// 상수버퍼 레지스터
cbuffer TRANSFORM : register(b0)
{
    row_major matrix g_matWorld;    // 월드 스페이스 변환
    row_major matrix g_matView;     // View 스페이스 변환
    row_major matrix g_matProj;     // 투영 변환 (뷰 스페이스의 좌표를 -1~ 1의 NDC 좌표계로 투영 시켜야 한다)
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
    
    // 입력으로 들어온 정점좌표에 월드 행렬 변환 -> 뷰 행렬 변환 -> 투영 행렬 변환
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);

    //// 원근 투영일 경우 투영행렬까지 곱해준게 최종 NDC 좌표계가 되는 것이 아니다.
    //// xyz좌표의 z값을 기억하고있는 w로 각각 나눠줘야 한다. 단, 셰이더의 레스터 라이저 과정에서 알아서 나눠서 사용한다.
    //vProjPos.x /= vProjPos.w;
    //vProjPos.y /= vProjPos.w;
    //vProjPos.z /= vProjPos.w;
    
    output.vPosition = vProjPos;
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