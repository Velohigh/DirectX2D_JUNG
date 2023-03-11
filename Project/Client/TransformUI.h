#pragma once
#include "ComponentUI.h"

class CGameObject;

class TransformUI :
    public ComponentUI
{
private:
    bool    m_bAbsoluteCheck;

public:
    virtual int render_update() override;

public:
    TransformUI();
    ~TransformUI();
};

