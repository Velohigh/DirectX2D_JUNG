#pragma once
#include "UI.h"

class TreeUI;

class OutlinerUI :
    public UI
{
private:
    TreeUI* m_Tree;

public:
    virtual void tick() override;
    void ResetOutliner();


public:
    virtual int render_update() override;

public:
    OutlinerUI();
    ~OutlinerUI();
};

