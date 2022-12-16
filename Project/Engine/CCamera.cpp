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
	// View ��� ���
	// ==============
	m_matView = XMMatrixIdentity();
	
	// ī�޶� ��ǥ�� �������� �̵�
	Vec3 vCamPos = Transform()->GetRelativePos();
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);	// ����ķ� ���� �̵���� ���Ѱ�
	Matrix matViewRot = XMMatrixIdentity();	// ī�޶��� ȸ�����, 2D ��� X

	// 2D������ ī�޶� Z�ุ �ٶ󺻴ٰ� �����ϰ�,
	// ȸ���� �ʿ���⋚����, ī�޶� �������� �̵���Ű��, �̸� ��ü�鿡 �����Ű��, ī�޶� �������� �̵��ϴ� ��ŭ
	// ��ü���� ���󰡰�, �ű⼭ ��, ī�޶��� ȸ���� 2D�������δ� �ʿ���� ����.
	m_matView = matViewTrans * matViewRot;


	// ==============
	// ���� ��� ���
	// ==============
	m_matProj = XMMatrixIdentity();

	m_fAspectRatio;

	// ���� ����
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	// �־��� �ػ󵵸� -1 ~ 1 �� NDC ��ǥ��� ���� ���ִ� ����� ��ȯ�ϴ� �Լ�.
	m_matProj = XMMatrixOrthographicLH(vResolution.x, vResolution.y, 1.f, 10000.f);


	// ���� ����
	//m_matProj = XMMatrixPerspectiveFovLH(XM_PI / 2.f, m_fAspectRatio, 1.f, 10000.f);


	// ��� ������Ʈ
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

}
