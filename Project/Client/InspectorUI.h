#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;

class InspectorUI :
    public UI
{
private:
    CGameObject* m_pTarget;
    ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END];


public:
    virtual void tick() override;
    virtual int render_update() override;

public:
    void SetTargetObject(CGameObject* _Target) { m_pTarget = _Target; }




public:
    InspectorUI();
    ~InspectorUI();
};

