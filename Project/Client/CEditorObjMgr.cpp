#include "pch.h"
#include "CEditorObjMgr.h"

#include "CGameObjectEx.h"

CEditorObjMgr::CEditorObjMgr()
{

}
CEditorObjMgr::~CEditorObjMgr()
{

}

void CEditorObjMgr::init()
{
	// 디버그 셰이프 생성
	
}

void CEditorObjMgr::progress()
{
	tick();
	render();
}

void CEditorObjMgr::tick()
{


	for (size_t i = 0; i < m_vecDebugShape.size(); ++i)
	{
		m_vecDebugShape[i]->tick();
	}

	for (size_t i = 0; i < m_vecDebugShape.size(); ++i)
	{
		m_vecDebugShape[i]->finaltick();
	}

}

void CEditorObjMgr::render()
{
	for (size_t i = 0; i < m_vecDebugShape.size(); ++i)
	{
		m_vecDebugShape[i]->render();
	}
}
