#include "pch.h"
#include "ContentUI.h"

#include <Engine\CResMgr.h>
#include "TreeUI.h"

ContentUI::ContentUI()
    : UI("##Content")
{
    SetName("ContentUI");

    // ContentUI 안에 자식으로 Tree 를 추가한다.
    m_Tree = new TreeUI;
    m_Tree->SetName("ContentTree");
    m_Tree->SetActive(true);
    m_Tree->ShowRoot(false);
    AddChildUI(m_Tree);

    // 루트 생성
    m_Tree->AddItem("RootNode", 0);

    // 루트 밑에 자식 2개 추가
    TreeNode* pChild1Node = m_Tree->AddItem("Child1", 0);
    m_Tree->AddItem("Child2", 0);

    // Child1 밑에 자식 2개 추가
    m_Tree->AddItem("ChildChild1", 0, pChild1Node);
    m_Tree->AddItem("ChildChild2", 0, pChild1Node);
}

ContentUI::~ContentUI()
{

}


int ContentUI::render_update()
{

    return 0;
}

void ContentUI::ResetContent()
{
    // Tree Clear
    m_Tree->Clear();

    // 리소스 매니저에서 현재 모든 리소스 목록 받아옴

    // m_Tree 에 현재 리소스 목록을 AddItem

}

