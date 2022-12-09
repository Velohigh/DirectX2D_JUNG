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
	// 메쉬나 셰이더를 지정안했다면 출력 X
	if (nullptr == m_pMesh || nullptr == m_pShader)
		return;

	// Transform 에 UpdateData 요청 (위치값 레지스터b0에 세팅하라고 요청)
	Transform()->UpdateData();


	m_pShader->UpdateData();	// 원하는 셰이더 선택
	m_pMesh->render();			// 그것을 출력할 메쉬정하고 출력


}
