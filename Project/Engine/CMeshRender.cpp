#include "pch.h"
#include "CMeshRender.h"

#include "CGraphicsShader.h"
#include "CMesh.h"


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


	m_pShader->UpdateData();	// ���ϴ� ���̴� ����
	m_pMesh->render();			// �װ��� ����� �޽����ϰ� ���
}
