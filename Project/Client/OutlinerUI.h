#pragma once
#include "UI.h"

class TreeUI;
class TreeNode;

class OutlinerUI :
    public UI
{
private:
    TreeUI* m_Tree;

public:
    virtual void tick() override;
    virtual int render_update() override;

public:
    void ResetOutliner();
    void SetTargetToInspector(DWORD_PTR _SelectedNode);

private:
    void AddGameObject(CGameObject* _Obj, TreeNode* _ParentNode);
    void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode);


public:
    OutlinerUI();
    ~OutlinerUI();
};

