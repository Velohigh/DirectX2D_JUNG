#pragma once
#include "UI.h"

class CGameObject;

class TransformUI :
    public UI
{
private:
    CGameObject* m_Target;

public:
    virtual void render_update() override;

public:
    void SetTarget(CGameObject* _Target) { m_Target = _Target; }

public:
    TransformUI();
    ~TransformUI();
};

