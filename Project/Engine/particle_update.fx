#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);

#define ParticleMaxCount g_int_0

[numthreads(128, 1, 1)]
void CS_ParticleUpdate(int3 _ID : SV_DispatchThreadID)
{
    if (ParticleMaxCount <= _ID.x)
        return;
    
    tParticle particle = ParticleBuffer[_ID.x];
    
    particle.vWorldPos += particle.vVelocity * g_DT;
    
    ParticleBuffer[_ID.x] = particle;
}

#endif


