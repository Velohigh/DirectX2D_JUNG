#pragma once
#include "UI.h"

class ListUI :
    public UI
{
private:


public:
    virtual int render_update() override;

public:
    ListUI();
    ~ListUI();
};

