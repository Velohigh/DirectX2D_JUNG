#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CTransform.h"




CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_fAspectRatio(1.f)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
{
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	m_fAspectRatio = vRenderResolution.x / vRenderResolution.y;
}

CCamera::~CCamera()
{
}

void CCamera::finaltick()
{

	// ==============
	// View 행렬 계산
	// ==============
	m_matView = XMMatrixIdentity();
	
	// 카메라 좌표를 원점으로 이동
	Vec3 vCamPos = Transform()->GetRelativePos();
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);	// 뷰행렬로 갈때 이동행렬 구한것

	// 카메라가 바라보는 방향을 Z 축과 평행하게 만드는 회전 행렬을 적용
	Matrix matViewRot = XMMatrixIdentity();


	Vec3 vR = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);
	Vec3 vU = Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vF = Transform()->GetRelativeDir(DIR_TYPE::FRONT);

	matViewRot._11 = vR.x;	matViewRot._12 = vU.x; matViewRot._13 = vF.x;
	matViewRot._21 = vR.y;	matViewRot._22 = vU.y; matViewRot._23 = vF.y;
	matViewRot._31 = vR.z;	matViewRot._32 = vU.z; matViewRot._33 = vF.z;

	// 최종 뷰행렬
	// 카메라를 원점으로 이동시키고, 카메라가 z축을 바라보도록 회전
	m_matView = matViewTrans * matViewRot;


	// ==============
	// 투영 행렬 계산
	// ==============
	m_matProj = XMMatrixIdentity();

	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 직교 투영
		Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
		// 넣어준 해상도를 -1 ~ 1 의 NDC 좌표계로 투영 해주는 행렬을 반환하는 함수.
		m_matProj = XMMatrixOrthographicLH(vResolution.x, vResolution.y, 1.f, 10000.f);
	}
	else
	{
		// 원근 투영
		m_matProj = XMMatrixPerspectiveFovLH(XM_PI / 2.f, m_fAspectRatio, 1.f, 10000.f);
	}


	// 행렬 업데이트
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

}
