#pragma once

#include "CRenderComponent.h"

class CTileMap :
    public CRenderComponent
{
private:
    UINT    m_iTileCountX;  // 타일 가로
    UINT    m_iTileCountY;  // 타일 세로

public:
    virtual void finaltick() override;
    virtual void render() override;

    void UpdateData();

    CLONE(CTileMap);
public:
    CTileMap();
    ~CTileMap();
};

