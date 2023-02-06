#ifndef _STRUCT
#define _STRUCT


struct tLightColor
{
    float4 vDiffuse; // 빛의 색상
    float4 vAmbient; // 주변 광(환경 광)
};

// LightInfo
struct tLightInfo
{
    tLightColor Color; // 빛의 색상
    
    float4 vWorldPos; // 광원의 월드 스페이스 위치
    float4 vWorldDir; // 빛을 보내는 방향
    
    uint LightType; // 빛의 타입(방향성, 점, 스포트)
    float Radius; // 빛의 반경(사거리)
    float Angle; // 빛의 각도    
    int Padding;
};


// Particle
struct tParticle
{
    float4 vWorldPos; // 파티클 위치
    float4 vWorldScale; // 파티클 크기
    float4 vColor; // 파티클 색상
    float4 vVelocity; // 파티클 현재 속도
    float4 vForce; // 파티클에 주어진 힘

    float Age; // 생존 시간
    float NomalizedAge; // 수명대비 생존시간을 0~1로 정규화 한 값
    float LifeTime; // 수명
    float Mass; // 질량
};


struct tParticleModule
{
	// 스폰 모듈
    float4 vSpawnColor;
    float4 vSpawnScale;
    float3 vBoxShapeScale;
    int iMaxParticleCount;
    float fSphereShapeRadius;
    int SpawnShapeType; // Sphere , Box

	// Color Change 모듈
    float4 vStartColor; // 초기 색상
    float4 vEndColor; // 최종 색상

	// Scale Change 모듈
    float4 vStartScale; // 초기 크기
    float4 vEndScale; // 최종 크기	

	// Module Check
    int ModuleCheck[3];
};

#endif
