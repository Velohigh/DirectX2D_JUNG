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
    TreeNode* m_ParentNode;   // 부모노드
    vector<TreeNode*>   m_vecChildNode; // 노드의 자식 노드

    string              m_strName;      // 노드의 출력 이름
    UINT                m_ID;           // 노드의 고유 ID

    DWORD_PTR           m_Data;         // 노드에 저장된 데이터


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
    UINT        g_NextId;   // 생성되는 노드뒤에 붙여줄 고유 숫자

    bool        m_bShowRoot;

public:
    virtual int render_update() override;
public:
    void Clear();
    TreeNode* AddItem(const string& _strNodeName, DWORD_PTR _Data, TreeNode* _pParent = nullptr);
    void ShowRoot(bool _Show) { m_bShowRoot = _Show; }

public:
    TreeUI();
    ~TreeUI();
};

