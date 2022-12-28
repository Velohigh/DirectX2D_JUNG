#pragma once


// 정점 구조체
struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};
typedef tVertex Vtx;


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