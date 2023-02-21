#pragma once
#include "ResUI.h"
class MaterialUI :
    public ResUI
{


public:
    virtual int render_update() override;

public:
    MaterialUI();
    ~MaterialUI();
};

