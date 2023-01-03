#pragma once


// 정점 구조체
struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};
typedef tVertex Vtx;

// Event
struct tEvent
{
	EVENT_TYPE	Type;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};

struct tDebugShapeInfo
{
	SHAPE_TYPE	eShape;
	Matrix		matWorld;
	Vec3		vWorldPos;
	Vec3		vWorldScale;
	Vec3		vWorldRotation;
	Vec4		vColor;
	float		fMaxTime;	// 이미지가 유지되는 시간
	float		fCurTime;
};






struct tLightColor
{
	Vec4 vDiffuse;	// 빛의 색상
	Vec4 vAmbient;	// 주변 광(환경 광)
};

// LightInfo
struct tLightInfo
{
	tLightColor Color;		 // 빛의 색상
	Vec3		vWorldPos;   // 광원의 월드 스페이스 위치
	Vec3		vWorldDir;	 // 빛을 보내는 방향
	float		Radius;		 // 빛의 반경(사거리)
	float		Angle;		 // 빛의 각도
	UINT		LightType;   // 빛의 타입(방향성, 점, 스포트)

	int			Padding[3];
};






// ===================
// 상수버퍼 대응 구조체
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_transform;

// 재질을 통해 상수 값을 전달할 것임
struct tMtrlConst
{
	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];

	int arrTex[(UINT)TEX_PARAM::TEX_END];	// 해당 텍스쳐 레지스터 세팅 여부
};