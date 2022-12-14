#pragma once
#include "CComponent.h"


class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos;     // 위치
    Vec3    m_vRelativeScale;   // 크기
    Vec3    m_vRelativeRot;     // 회전

    Matrix  m_matWorld;     // 크기, 회전, 이동 정보를 합쳐놓음

public:
    void SetRelativePos(Vec3 _vPos) { m_vRelativePos = _vPos; }
    void SetRelativeScale(Vec3 _vScale) { m_vRelativeScale = _vScale; }
    void SetRelativeRot(Vec3 _vRot) { m_vRelativeRot = _vRot; }

    void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); }
    void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); }
    void SetRelativeRot(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z); }

    Vec3 GetRelativePos() { return m_vRelativePos; }
    Vec3 GetRelativeScale() { return m_vRelativeScale; }
    Vec3 GetRelativeRot() { return m_vRelativeRot; }




public:
    virtual void finaltick() override;
    void UpdateData();

    CLONE(CTransform);
public:
    CTransform();
    ~CTransform();
};

