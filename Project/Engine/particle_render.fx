#ifndef _PARTICLE_RENDER
#define _PARTICLE_RENDER

#include "value.fx"
#include "struct.fx"


// ========================
// Particle Render Shader
// mesh : RectMesh

// Parameter
// g_int_0 : Particle Index
// =========================

StructuredBuffer<tParticle> ParticleBuffer : register(t20);


struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float3 vPos : POSITION;
};

VS_OUT VS_ParticleRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
     
    output.vPos = _in.vPos;
    
    return output;
}

// GeometryShader 사용
// 1. 파이프라인 제어
// 2. 빌보드 처리 (카메라를 바라보는..)
struct GS_OUT
{
    float3 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

[maxvertexcount(6)]
void GS_ParticleRender(VS_OUT _in, uint _iInstID : SV_InstanceID, inout TriangleStream<GS_OUT> _outstream)
{
    if (ParticleBuffer[_iInstID].Age < 0.f)
        return;

    float3 vParticleViewPos = mul(float4(ParticleBuffer[_iInstID].vWorldPos.xyz, 1.f), g_matView).xyz;
    float2 vParticleScale = ParticleBuffer[_iInstID].vWorldScale;
    
    // 0 -- 1
    // |    |
    // 3 -- 2
    float3 NewPos[4] =
    {
        float3(vParticleViewPos.x - vParticleScale.x / 2.f, vParticleViewPos.y + vParticleScale.y / 2.f, vParticleViewPos.z),
        float3(vParticleViewPos.x + vParticleScale.x / 2.f, vParticleViewPos.y + vParticleScale.y / 2.f, vParticleViewPos.z),
        float3(vParticleViewPos.x + vParticleScale.x / 2.f, vParticleViewPos.y - vParticleScale.y / 2.f, vParticleViewPos.z),
        float3(vParticleViewPos.x - vParticleScale.x / 2.f, vParticleViewPos.y - vParticleScale.y / 2.f, vParticleViewPos.z)
    };
    
    // 정점 생성
    //_outstream.Append();    
}


float4 PS_ParticleRender(GS_OUT _in) : SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f);
}


#endif
