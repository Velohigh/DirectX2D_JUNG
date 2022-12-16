#pragma once
#include "CComponent.h"

class CCamera :
    public CComponent

{
private:


    float m_fAspectRatio;   // Á¾È¾ºñ

    Matrix m_matView;   // ºä Çà·Ä
    Matrix m_matProj;   // Åõ¿µ Çà·Ä

public:
    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }



public:
    virtual void finaltick() override;

    CLONE(CCamera);
public:
    CCamera();
    ~CCamera();

};

