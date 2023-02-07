#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<int4> ParticleSpawnCount : register(u1);
StructuredBuffer<tParticleModule> ParticleModuleData : register(t20);

#define ParticleMaxCount g_int_0
#define SpawnCount ParticleSpawnCount[0].x
#define ModuleData ParticleModuleData[0]


[numthreads(128, 1, 1)]
void CS_ParticleUpdate(int3 _ID : SV_DispatchThreadID)
{
    // 스레드 ID 가 파티클버퍼 최대 수를 넘긴경우 or 스레드 담당 파티클이 비활성화 상태인 경우
    if (ParticleMaxCount <= _ID.x)
        return;
        
    tParticle particle = ParticleBuffer[_ID.x];
        
    // 파티클이 비활성화 상태인 경우
    if (particle.Age < 0.f)
    {
        // SpawnCount 를 확인
        // 만약 SpawnCount 가 0 이상이라면, 파티클을 활성화시킴      
        while (0 < SpawnCount)
        {
            int orgvalue = SpawnCount;
            int outvalue = 0;
            InterlockedCompareExchange(SpawnCount, orgvalue, SpawnCount - 1, outvalue);
            
            if (orgvalue == outvalue)
            {
                particle.Age = 0.f;
                particle.LifeTime = 10.f;
                break;
            }
        }
    }
    
    // 파티클이 활성화인 경우
    else
    {
        // 속도에 따른 파티클위치 이동
        particle.vWorldPos += particle.vVelocity * g_DT;
        
        // 파티클의 Age 에 시간을 누적시킴
        particle.Age += g_DT;
        
        // 파티클의 수명이 끝나면, 다시 비활성화 상태로 되돌림
        if (particle.LifeTime <= particle.Age)
        {
            particle.Age = -1.f;
        }
    }
    
    
    // 변경점 적용
    ParticleBuffer[_ID.x] = particle;
}

#endif


