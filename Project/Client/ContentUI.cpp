#include "pch.h"
#include "ContentUI.h"

#include "TreeUI.h"

ContentUI::ContentUI()
    : UI("##Content")
{
    SetName("ContentUI");

    // ContentUI 안에 자식으로 Tree 를 추가한다.
    TreeUI* pTreeUI = new TreeUI;
    pTreeUI->SetName("ContentTree");
    pTreeUI->SetActive(true);
    AddChildUI(pTreeUI);
}

ContentUI::~ContentUI()
{
}


int ContentUI::render_update()
{

    return 0;
}

