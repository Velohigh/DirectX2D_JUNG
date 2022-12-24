#ifndef _TEST
#define _TEST

#include "value.fx"


// VS �Է� ����ü
struct VS_IN
{
	float3 vPos     : POSITION;
	float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD;
};

// ��ȯ ����ü
struct VS_OUT
{
    float4 vPosition    : SV_Position;
    float4 vOutColor    : COLOR;
    float2 vOutUV       : TEXCOORD;
};

// vertex shader
// LocalSpace ��ü�� -> ���� �����̽� -> ī�޶� -> NDC ��ǥ��� �̵�
VS_OUT VS_Test(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // �Է����� ���� ������ǥ�� ���� ��� ��ȯ -> �� ��� ��ȯ -> ���� ��� ��ȯ
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);

    //// ���� ������ ��� ������ı��� �����ذ� ���� NDC ��ǥ�谡 �Ǵ� ���� �ƴϴ�.
    //// xyz��ǥ�� z���� ����ϰ��ִ� w�� ���� ������� �Ѵ�. ��, ���̴��� ������ ������ �������� �˾Ƽ� ������ ����Ѵ�.
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