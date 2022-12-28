#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"


CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_vRelativeDir{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)}
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	// 크기 행렬
	Matrix matScale = XMMatrixIdentity();	// 단위 행렬 반환 함수
	matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);	// 크기 행렬 반환해주는 함수

	// 회전 행렬
	Matrix matRot = XMMatrixIdentity();
	matRot = XMMatrixRotationX(m_vRelativeRot.x);
	matRot *= XMMatrixRotationY(m_vRelativeRot.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	// 이동 행렬
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	// 월드 행렬
	m_matWorld = matScale * matRot * matTranslation;

	Vec3 vDefaultDir[3] = {
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], matRot);	// 동차 좌표를 0으로 확장하여 계산
	}

	// 부모 오브젝트가 있으면, 본인 월드행렬 * 부모 월드행렬
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		m_matWorld *= pParent->Transform()->m_matWorld;
	}

}

void CTransform::UpdateData()
{
	// 트랜스폼 바인딩 함수에서 하는일
	// 렌더링하기전에 본인의 좌표를 상수 버퍼에 저장 및 바인딩

	// 위치값을 상수버퍼에 전달 및 바인딩
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_transform.matWorld = m_matWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	pTransformBuffer->SetData(&g_transform);	// 상수버퍼 세팅
	pTransformBuffer->UpdateData();	// 상수버퍼 바인딩

}
