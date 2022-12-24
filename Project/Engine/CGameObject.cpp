#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CMeshRender.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
{
}

CGameObject::~CGameObject()
{
	/*for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			delete m_arrCom[i];
	}*/

	Safe_Del_Array(m_arrCom);
}


void CGameObject::tick()
{
	for(UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->tick();
		}
	}
}

void CGameObject::finaltick()
{
	// Script 는 finaltick() 이 호출되지 않는다!
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->finaltick();
		}
	}
}

void CGameObject::render()
{
	if (nullptr != m_RenderCom)
		m_RenderCom->render();
}

void CGameObject::AddComponent(CComponent* _Component)
{
	// 이미 보유하고 있는 컴포넌트인 경우
	assert(!m_arrCom[(UINT)_Component->GetType()]);

	_Component->m_pOwner = this;
	m_arrCom[(UINT)_Component->GetType()] = _Component;

	// RenderComponent 확인,   렌더 컴포넌트는 딱 하나만 가져야 한다.
	if (COMPONENT_TYPE::MESHRENDER <= _Component->GetType()
		&& _Component->GetType() <= COMPONENT_TYPE::DECAL)
	{
		assert(!m_RenderCom);
		m_RenderCom = (CRenderComponent*)_Component;
	}
}