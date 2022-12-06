#ifndef _TEST
#define _TEST

// 상수버퍼 레지스터
cbuffer TRANSFORM : register(b0)
{
    float4 vPlayerPos;
};

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
    float4 vOutColor : COLOR;
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
    
    return output;
}

// pixel shader
float4 PS_Test(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = _in.vOutColor;
    
    return vColor;
}
#endif