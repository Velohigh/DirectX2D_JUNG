#include "pch.h"
#include "TreeUI.h"

// ========
// TreeNode
// ========
TreeNode::TreeNode()
{

}

TreeNode::~TreeNode()
{

}

void TreeNode::render_update()
{
    ImGuiTreeNodeFlags_ flag = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_None;

    if (ImGui::TreeNodeEx("Parent", flag))
    {
        for (size_t i = 0; i < m_vecChildNode.size(); ++i)
        {
            m_vecChildNode[i]->render_update();
        }

        ImGui::TreePop();
    }
}









// ================
//      TreeUI
// ================
TreeUI::TreeUI()
    : UI("##Tree")
{

}

TreeUI::~TreeUI()
{
}

int TreeUI::render_update()
{
    m_RootNode->render_update();

    return 0;
}

