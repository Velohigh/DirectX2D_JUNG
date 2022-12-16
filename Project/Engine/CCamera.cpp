#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CTransform.h"




CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_fAspectRatio(1.f)
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
	Matrix matViewRot = XMMatrixIdentity();	// 카메라의 회전행렬, 2D 사용 X

	// 2D에서는 카메라가 Z축만 바라본다고 가정하고,
	// 회전이 필요없기떄문에, 카메라를 원점으로 이동시키고, 이를 물체들에 적용시키면, 카메라가 원점으로 이동하는 만큼
	// 물체들이 따라가고, 거기서 끝, 카메라의 회전이 2D기준으로는 필요없기 때문.
	m_matView = matViewTrans * matViewRot;


	// ==============
	// 투영 행렬 계산
	// ==============
	m_matProj = XMMatrixIdentity();

	m_fAspectRatio;

	// 직교 투영
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	// 넣어준 해상도를 -1 ~ 1 의 NDC 좌표계로 투영 해주는 행렬을 반환하는 함수.
	m_matProj = XMMatrixOrthographicLH(vResolution.x, vResolution.y, 1.f, 10000.f);


	// 원근 투영
	//m_matProj = XMMatrixPerspectiveFovLH(XM_PI / 2.f, m_fAspectRatio, 1.f, 10000.f);


	// 행렬 업데이트
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

}
