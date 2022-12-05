#ifndef _TEST
#define _TEST

// VS 입력 구조체
struct VS_IN
{
	float3 vPos : POSITION;
	float4 vColor : COLOR;
};

// 반환 구조체
struct VS_OUT
{
    float4 vPosition : SV_Position;
};

// vertex shader
VS_OUT VS_Test(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos, 1.f);
    
    return output;
}

// pixel shader
float4 PS_Test(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = float4(1.f, 0.f, 0.f, 1.f);
    
    return vColor;
}
#endif