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
    void AddComponent(COMPONENT_TYPE _type);


public:
    MenuUI();
    ~MenuUI();
};

