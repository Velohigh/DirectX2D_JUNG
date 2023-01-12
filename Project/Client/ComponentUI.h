#pragma once
#include "UI.h"

class ComponentUI :
    public UI
{
private:
    CGameObject* m_Target;

public:
    void SetTarget(CGameObject* _Target) { m_Target = _Target; }

public:
    ComponentUI(const string& _Name);
    ~ComponentUI();
};

