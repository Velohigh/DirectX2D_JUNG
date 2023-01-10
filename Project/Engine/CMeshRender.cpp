#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
}

void CMeshRender::render()
{
	// �޽��� ���͸����� �������ߴٸ� ��� X
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	// Transform �� UpdateData ��û (��ġ�� ��������b0�� �����϶�� ��û)
	Transform()->UpdateData();

	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// ���� ������Ʈ
	GetMaterial()->UpdateData();	// Material �� ���ε�

	// ����
	GetMesh()->render();	// ����� �޽����ϰ� ���
	
}
