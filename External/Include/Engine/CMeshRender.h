#pragma once
#include "CComponent.h"
class CMeshRender :
    public CComponent
{
private:


public:
    virtual void finaltick() override;






    CLONE(CMeshRender);
public:
    CMeshRender();
    ~CMeshRender();

};

