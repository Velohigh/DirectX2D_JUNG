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
};


// 재질을 통해 상수 값을 전달할 것임
struct tMtrlConst
{
	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];
};