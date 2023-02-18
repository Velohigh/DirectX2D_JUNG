#pragma once
#include "UI.h"

class TreeUI;

class ContentUI :
    public UI
{
private:
    TreeUI* m_Tree;

public:
    virtual int render_update() override;

public:
    void ResetContent();

public:
    ContentUI();
    ~ContentUI();
};

