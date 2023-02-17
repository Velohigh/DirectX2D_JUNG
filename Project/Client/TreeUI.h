#pragma once
#include "UI.h"

// =========
// TreeNode
// =========
class TreeUI;
class TreeNode
{
private:
    TreeUI* m_Owner;        // 노드를 소유하고 있는 트리
    vector<TreeNode*>   m_vecChildNode; // 노드의 자식 노드


private:
    void render_update();

public:
    TreeNode();
    ~TreeNode();

    friend class TreeUI;
};



// ======
// TreeUI
// ======
class TreeUI :
    public UI
{
private:
    TreeNode* m_RootNode; // 트리가 소유하고 있는 노드 중 루트 노드


public:
    virtual int render_update() override;


public:
    TreeUI();
    ~TreeUI();
};

