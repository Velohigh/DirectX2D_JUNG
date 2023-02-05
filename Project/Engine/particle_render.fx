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
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_ParticleRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // Local Mesh 의 정점에 파티클 배율을 곱하고 월드 위치로 이동시킨다.
    float3 vWorldPos = _in.vPos * ParticleBuffer[_in.iInstID].vWorldScale.xyz + ParticleBuffer[_in.iInstID].vWorldPos.xyz;
    
    // View, Proj 행렬을 곱해서 NDC 좌표계로 이동시킨다.
    float4 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    output.vPosition = mul(vViewPos, g_matProj);
    
    // UV 전달
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_ParticleRender(VS_OUT _in) : SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f);
}


#endif
