#ifndef _TEST
#define _TEST

#include "value.fx"


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
    
    if (g_int_0 == 0)
        vColor = g_tex_0.Sample(g_sam_0, _in.vOutUV);
    else if (g_int_0 == 1)
        vColor = g_tex_0.Sample(g_sam_1, _in.vOutUV);
    
    return vColor;
}

#endif