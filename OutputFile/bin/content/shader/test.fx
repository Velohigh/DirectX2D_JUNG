#ifndef _TEST
#define _TEST

#include "value.fx"


// VS �Է� ����ü
struct VS_IN
{   
    float3 vPos     : POSITION; // semantic  
    float4 vColor   : COLOR;    
    float2 vUV      : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vOutColor : COLOR;
    float2 vOutUV    : TEXCOORD;
};

// vertex shader
// LocalSpace ��ü�� NDC ��ǥ��� �̵�
VS_OUT VS_Test(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // �Է����� ���� ������ǥ�� ������� ���� ���ؼ� ���    
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);
       
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





//struct PS_OUT
//{
//    float4 vColor : SV_Target;
//};

//PS_OUT PS_Test(VS_OUT _in)
//{
//    PS_OUT output = (PS_OUT) 0.f;
    
//    output.vColor = float4(1.f, 0.f, 0.f, 1.f);
    
//    return output;
//}