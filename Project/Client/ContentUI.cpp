#include "pch.h"
#include "ContentUI.h"

#include <Engine\CResMgr.h>
#include "TreeUI.h"

#include <Engine\CKeyMgr.h>

ContentUI::ContentUI()
    : UI("##Content")
{
    SetName("ContentUI");

    // ContentUI �ȿ� �ڽ����� Tree �� �߰��Ѵ�.
    m_Tree = new TreeUI;
    m_Tree->SetName("ContentTree");
    m_Tree->SetActive(true);
    m_Tree->ShowRoot(false);
    AddChildUI(m_Tree);

    // ��Ʈ ����
    m_Tree->AddItem("RootNode", 0);

    // ��Ʈ �ؿ� �ڽ� 2�� �߰�
    TreeNode* pChild1Node = m_Tree->AddItem("Child1", 0);
    m_Tree->AddItem("Child2", 0);

    // Child1 �ؿ� �ڽ� 2�� �߰�
    m_Tree->AddItem("ChildChild1", 0, pChild1Node);
    m_Tree->AddItem("ChildChild2", 0, pChild1Node);
}

ContentUI::~ContentUI()
{

}


int ContentUI::render_update()
{
    if (KEY_TAP(KEY::SPACE))
    {
        ResetContent();
    }
    return 0;
}

void ContentUI::ResetContent()
{
    // Tree Clear
    m_Tree->Clear();
    m_Tree->AddItem("Root", 0);

    // ���ҽ� �Ŵ������� ���� ��� ���ҽ� ��� �޾ƿ�
    for (size_t i = 0; i < (UINT)RES_TYPE::END; ++i)
    {
        const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

        // m_Tree �� ���� ���ҽ� ����� AddItem
        TreeNode* pCategory = m_Tree->AddItem(ToString((RES_TYPE)i), 0);
        pCategory->SetCategoryNode(true);

        for (const auto& pair : mapRes)
        {
            m_Tree->AddItem(string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get(), pCategory);
        }
    }
}


