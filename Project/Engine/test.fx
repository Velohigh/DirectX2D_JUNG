#ifndef _TEST
#define _TEST

// ������� ��������
cbuffer TRANSFORM : register(b0)
{
    float4 vPlayerPos;
};

Texture2D g_tex_0 : register(t0);

// �̹������� ������ �������� ���� ���÷�
SamplerState g_sam_0 : register(s0);

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
    
    // �Է����� ���� ������ǥ�� ������� ���� ���ؼ� ��� ( ���� �����̽� ��ȯ )
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
    //PS �ܰ迡��, ���� �ȼ��鿡�� ������ ����(����)�� �ƴ�, �ش� �ؽ��Ŀ��� �����´� (���ø�)
    vColor = g_tex_0.Sample(g_sam_0, _in.vOutUV); 
    
    return vColor;
}
#endif