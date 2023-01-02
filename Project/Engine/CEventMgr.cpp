#include "pch.h"
#include "CEventMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CGameObject.h"


CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{

}

void CEventMgr::tick()
{
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		switch (m_vecEvent[i].Type)
		{
		// wParam : GameObject, lParam : Layer Index
		case EVENT_TYPE::CREATE_OBJECT:
		{
			CGameObject* NewObject = (CGameObject*)m_vecEvent[i].wParam;
			int iLayerIdx = (int)m_vecEvent[i].lParam;

			CLevelMgr::GetInst()->GetCurLevel()->AddGameObject(NewObject, iLayerIdx, false);
		}
			break;
		case EVENT_TYPE::DELETE_OBJECT:


			break;
		case EVENT_TYPE::ADD_CHILD:


			break;
		case EVENT_TYPE::DELETE_RESOURCE:


			break;
		case EVENT_TYPE::LEVEL_CHANGE:


			break;
		}
	}

	m_vecEvent.clear();
}
