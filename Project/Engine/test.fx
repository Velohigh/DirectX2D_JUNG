#ifndef _TEST
#define _TEST

// 상수버퍼 레지스터
cbuffer TRANSFORM : register(b0)
{
    float4 vPlayerPos;
};

Texture2D g_tex_0 : register(t0);

// 이미지에서 색상을 가져오기 위한 샘플러
SamplerState g_sam_0 : register(s0);

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
    
    //vColor = _in.vOutColor;
    //PS 단계에서, 이제 픽셀들에서 가져온 색상(보간)이 아닌, 해당 텍스쳐에서 가져온다 (샘플링)
    vColor = g_tex_0.Sample(g_sam_0, _in.vOutUV); 
    
    return vColor;
}
#endif