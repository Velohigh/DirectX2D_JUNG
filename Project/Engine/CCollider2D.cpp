#include "pch.h"
#include "CCollider2D.h"

#include "components.h"

CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_Shape(COLLIDER2D_TYPE::RECT)
	, m_bAbsolute(false)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::finaltick()
{
	m_matCollider2D = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	m_matCollider2D *= XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	const Matrix& matWorld = Transform()->GetWorldMat();

	if (m_bAbsolute)
	{
		Matrix matParentScaleInv = XMMatrixInverse(nullptr, Transform()->GetWorldScaleMat());
		m_matCollider2D = m_matCollider2D * matParentScaleInv  * matWorld;
	}
	else
	{
		// 충돌체 월드 * 오브젝트 월드
		m_matCollider2D *= matWorld;
	}

	// DebugShape 요청
	if (COLLIDER2D_TYPE::CIRCLE == m_Shape)
	{
		DrawDebugCircle(m_matCollider2D, Vec4(0.f, 1.f, 0.f, 1.f), 0.f);
	}
	else
	{
		DrawDebugRect(m_matCollider2D, Vec4(0.f, 1.f, 0.f, 1.f), 0.f);
	}
}
