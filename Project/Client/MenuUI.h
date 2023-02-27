#pragma once
#include "UI.h"


class MenuUI :
    public UI
{
private:


public:
    virtual void finaltick() override;
    virtual int render_update() override;


private:
    void CreateEmptyObject();


public:
    MenuUI();
    ~MenuUI();
};

