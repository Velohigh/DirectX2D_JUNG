#include "pch.h"
#include "OutlinerUI.h"

#include "ImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine\CEventMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>

#include "TreeUI.h"

OutlinerUI::OutlinerUI()
	: UI("##Outliner")
	, m_Tree(nullptr)
{
	SetName("Outliner");

	// OutlinerUI 안에 자식으로 Tree 를 추가한다.
	m_Tree = new TreeUI;
	m_Tree->SetName("OutlinerTree");
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(false);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&OutlinerUI::SetTargetToInspector);
	m_Tree->AddDynamic_DragDrop(this, (UI_DELEGATE_2)&OutlinerUI::DragDrop);

	AddChildUI(m_Tree);
}

OutlinerUI::~OutlinerUI()
{
}

int OutlinerUI::render_update()
{
	return 0;
}

void OutlinerUI::tick()
{
	if (CEventMgr::GetInst()->IsLevelChanged())
	{
		ResetOutliner();
	}
}

void OutlinerUI::ResetOutliner()
{
	// Tree Clear
	m_Tree->Clear();
	m_Tree->AddItem("Root", 0);

	// 리소스 매니저에서 현재 모든 리소스 목록 받아옴
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < (UINT)MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);

		const vector<CGameObject*>& vecParentObj = pLayer->GetParentObject();

		for (size_t i = 0; i < vecParentObj.size(); ++i)
		{
			AddGameObject(vecParentObj[i], nullptr);
		}
	}
}

void OutlinerUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
	CGameObject* pSelectObject = (CGameObject*)pSelectedNode->GetData();

	// Inspector 에 선택된 GameObject 를 알려준다.	
	InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetObject(pSelectObject);
}

void OutlinerUI::AddGameObject(CGameObject* _Obj, TreeNode* _ParentNode)
{
	// 오브젝트를 트리에 넣고, 생성된 노드 주소를 받아둔다.
	TreeNode* pNode = m_Tree->AddItem(string(_Obj->GetName().begin(), _Obj->GetName().end())
		, (DWORD_PTR)_Obj
		, _ParentNode);

	// 오브젝트의 자식오브젝트 들을 오브젝트 노드를 부모로 해서 그 밑으로 다시 넣어준다.
	const vector<CGameObject*>& vecChild = _Obj->GetChild();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(vecChild[i], pNode);
	}
}

void OutlinerUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
{
	TreeNode* pDragNode = (TreeNode*)_DragNode;
	TreeNode* pDropNode = (TreeNode*)_DropNode;

	pDragNode->GetData();
	pDropNode->GetData();
}