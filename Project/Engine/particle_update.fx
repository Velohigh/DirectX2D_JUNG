#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"
#include "func.fx"

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<int4> ParticleSpawnCount : register(u1);
StructuredBuffer<tParticleModule> ParticleModuleData : register(t20);
Texture2D NoiseTexture : register(t21);


#define ObjectPos           g_vec4_0

#define NoiseTexResolution  g_vec2_0

#define SpawnCount          ParticleSpawnCount[0].x
#define ModuleData          ParticleModuleData[0]
#define ParticleMaxCount    ParticleModuleData[0].iMaxParticleCount

#define SpawnModule         ParticleModuleData[0].Spawn
#define ColorChangeModule   ParticleModuleData[0].ColorChange
#define ScaleChangeModule   ParticleModuleData[0].ScaleChange


[numthreads(128, 1, 1)]
void CS_ParticleUpdate(int3 _ID : SV_DispatchThreadID)
{
    // 스레드 ID 가 파티클버퍼 최대 수를 넘긴경우 or 스레드 담당 파티클이 비활성화 상태인 경우
    if (ParticleMaxCount <= _ID.x)
        return;
        
    tParticle particle = ParticleBuffer[_ID.x];
           
    if (SpawnModule)
    {
        // 파티클이 비활성화 상태인 경우
        if (particle.Active == 0)
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
                    particle.Active = 1;
                    
                    // 랜덤 결과를 받을 변수
                    float3 vOut1 = (float3) 0.f;
                    float3 vOut2 = (float3) 0.f;
                    float3 vOut3 = (float3) 0.f;
                    
                    // 전체 유효 스레드의 아이디를 0 ~ 1 로 정규화
                    float fNormalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID, vOut1);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID + 0.1f, vOut2);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNormalizeThreadID + 0.2f, vOut3);
                    
                    // Box 스폰
                    if (ModuleData.SpawnShapeType == 0)
                    {
                        particle.vLocalPos.xyz = float3(ModuleData.vBoxShapeScale.x * vOut1.r - ModuleData.vBoxShapeScale.x * 0.5f
                                                      , ModuleData.vBoxShapeScale.y * vOut2.r - ModuleData.vBoxShapeScale.y * 0.5f
                                                      , ModuleData.vBoxShapeScale.z * vOut3.r - ModuleData.vBoxShapeScale.z * 0.5f);
                        particle.vWorldPos.xyz = particle.vLocalPos.xyz + ObjectPos.xyz;
                        
                        particle.vWorldScale.xyz = float3(10.f, 10.f, 1.f);
                    }
                    
                    // Sphere 스폰
                    else if (ModuleData.SpawnShapeType == 1)
                    {
                        
                    }
                    
                    
                    float fRadius = 500.f; //vOut1.r * 200.f;
                    float fAngle = vOut2.r * 2 * 3.1415926535f;
                    //particle.vWorldPos.xyz = float3(fRadius * cos(fAngle), fRadius * sin(fAngle), 100.f);                    
                   
                   
                    particle.Age = 0.f;
                    particle.LifeTime = 10.f;
                    break;
                }
            }
        }
    }
           
    
    // 파티클이 활성화인 경우
    if (particle.Active)
    {
        // 속도에 따른 파티클위치 이동
        if (ModuleData.Space == 0)
        {
            particle.vWorldPos += particle.vVelocity * g_DT;
        }
        else if (ModuleData.Space == 1)
        {
            particle.vLocalPos += particle.vVelocity * g_DT;
            particle.vWorldPos.xyz = particle.vLocalPos.xyz + ObjectPos.xyz;
        }
        
        
        
        // 파티클의 Age 에 시간을 누적시킴
        particle.Age += g_DT;
        
        // 파티클의 수명이 끝나면, 다시 비활성화 상태로 되돌림
        if (particle.LifeTime <= particle.Age)
        {
            particle.Active = 0.f;
        }
    }
    
    // 변경점 적용
    ParticleBuffer[_ID.x] = particle;
}

#endif



