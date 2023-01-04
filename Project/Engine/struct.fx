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
    
    float3 vWorldPos; // 광원의 월드 스페이스 위치
    uint LightType; // 빛의 타입(방향성, 점, 스포트)
    
    float3 vWorldDir; // 빛을 보내는 방향
    float Radius; // 빛의 반경(사거리)
    
    float Angle; // 빛의 각도    
    int Padding[3];
};





#endif
