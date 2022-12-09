#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"

CMeshRender::CMeshRender()
	: CComponent(COMPONENT_TYPE::MESHRENDER)
	, m_pMesh(nullptr)
	, m_pShader(nullptr)
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
	// �޽��� ���̴��� �������ߴٸ� ��� X
	if (nullptr == m_pMesh || nullptr == m_pShader)
		return;

	// Transform �� UpdateData ��û (��ġ�� ��������b0�� �����϶�� ��û)
	Transform()->UpdateData();


	m_pShader->UpdateData();	// ���ϴ� ���̴� ����
	m_pMesh->render();			// �װ��� ����� �޽����ϰ� ���


}
