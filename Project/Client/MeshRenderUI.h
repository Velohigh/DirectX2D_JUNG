#pragma once
#include "ComponentUI.h"

class MeshRenderUI :
    public ComponentUI
{

public:
    virtual int render_update() override;

public:
    MeshRenderUI();
    ~MeshRenderUI();
};

