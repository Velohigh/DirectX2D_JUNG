#pragma once
#include "UI.h"

// =========
// TreeNode
// =========
class TreeUI;
class TreeNode
{
private:
    TreeUI* m_Owner;        // ��带 �����ϰ� �ִ� Ʈ��
    vector<TreeNode*>   m_vecChildNode; // ����� �ڽ� ���


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
    TreeNode* m_RootNode; // Ʈ���� �����ϰ� �ִ� ��� �� ��Ʈ ���


public:
    virtual int render_update() override;


public:
    TreeUI();
    ~TreeUI();
};

