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
	// ũ�� ���
	Matrix matScale = XMMatrixIdentity();	// ���� ��� ��ȯ �Լ�
	matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);	// ũ�� ��� ��ȯ���ִ� �Լ�

	// ȸ�� ���
	Matrix matRot = XMMatrixIdentity();
	matRot = XMMatrixRotationX(m_vRelativeRot.x);
	matRot *= XMMatrixRotationY(m_vRelativeRot.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	// �̵� ���
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);


	Vec3 vDefaultDir[3] = {
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		// XMVector3TransformCoord(vDefaultDir[i], matRot);	// Coordinate ���� ��ǥ�� 1�� Ȯ���Ͽ� ���

		//m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], matRot);	// ���� ��ǥ�� 0���� Ȯ���Ͽ� ���
		//m_vRelativeDir[i].Normalize();
		
		m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], matRot);	// ���� ��ǥ�� 0���� Ȯ���Ͽ� ���
	}


	// ���� ���
	m_matWorld = matScale * matRot * matTranslation;
}

void CTransform::UpdateData()
{
	// Ʈ������ ���ε� �Լ����� �ϴ���
	// �������ϱ����� ������ ��ǥ�� ��� ���ۿ� ���� �� ���ε�

	// ��ġ���� ������ۿ� ���� �� ���ε�
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_transform.matWorld = m_matWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	pTransformBuffer->SetData(&g_transform);	// ������� ����
	pTransformBuffer->UpdateData();	// ������� ���ε�

}
