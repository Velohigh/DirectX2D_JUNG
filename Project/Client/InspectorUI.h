#pragma once
#include "UI.h"

class CGameObject;
class TransformUI;
class MeshRenderUI;

class InspectorUI :
    public UI
{
private:
    CGameObject* m_pTarget;
    TransformUI* m_TransformUI;
    MeshRenderUI* m_MeshRenderUI;

    //ComponentUI*    m_arrComUI[(UINT)COMPONENT_TYPE::END];


public:
    virtual void tick() override;
    virtual void render_update() override;

public:
    void SetTargetObject(CGameObject* _Target) { m_pTarget = _Target; }




public:
    InspectorUI();
    ~InspectorUI();
};

