#pragma once
#include "UI.h"

class ContentUI :
    public UI
{

public:
    virtual int render_update() override;

public:
    ContentUI();
    ~ContentUI();
};

