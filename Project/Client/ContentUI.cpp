#include "pch.h"
#include "ContentUI.h"

#include "TreeUI.h"

ContentUI::ContentUI()
    : UI("##Content")
{
    SetName("ContentUI");

    // ContentUI �ȿ� �ڽ����� Tree �� �߰��Ѵ�.
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

