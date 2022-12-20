#pragma once
#include "CComponent.h"

class CCamera :
    public CComponent

{
private:
    float m_fAspectRatio;   // 종횡비
    PROJ_TYPE   m_ProjType; // 투영 방식


    Matrix m_matView;   // 뷰 행렬
    Matrix m_matProj;   // 투영 행렬

public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }



public:
    virtual void finaltick() override;

    CLONE(CCamera);
public:
    CCamera();
    ~CCamera();

};

