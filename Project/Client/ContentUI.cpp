#include "pch.h"
#include "ContentUI.h"

#include <Engine\CResMgr.h>
#include "TreeUI.h"

#include <Engine\CKeyMgr.h>

ContentUI::ContentUI()
    : UI("##Content")
{
    SetName("Content");

    // ContentUI 안에 자식으로 Tree 를 추가한다.
    m_Tree = new TreeUI;
    m_Tree->SetName("ContentTree");
    m_Tree->SetActive(true);
    m_Tree->ShowRoot(false);
    AddChildUI(m_Tree);
}

ContentUI::~ContentUI()
{

}


void ContentUI::tick()
{
    UI::tick();

    if (CResMgr::GetInst()->IsResourceChanged())
    {
        ResetContent();
    }
}

int ContentUI::render_update()
{
    return 0;
}

void ContentUI::ResetContent()
{
    // Tree Clear
    m_Tree->Clear();
    m_Tree->AddItem("Root", 0);

    // 리소스 매니저에서 현재 모든 리소스 목록 받아옴
    for (size_t i = 0; i < (UINT)RES_TYPE::END; ++i)
    {
        const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

        // m_Tree 에 현재 리소스 목록을 AddItem
        TreeNode* pCategory = m_Tree->AddItem(ToString((RES_TYPE)i), 0);
        pCategory->SetCategoryNode(true);

        for (const auto& pair : mapRes)
        {
            m_Tree->AddItem(string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get(), pCategory);
        }
    }
}


