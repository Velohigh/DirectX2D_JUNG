#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);

[numthreads(128, 1, 1)]
void CS_ParticleUpdate(int3 _ID : SV_DispatchThreadID)
{
    ParticleBuffer[_ID.x];
}

#endif


