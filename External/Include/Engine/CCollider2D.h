#pragma once
#include "CComponent.h"

class CCollider2D :
    public CComponent
{
private:
    Vec3            m_vOffsetPos;
    Vec3            m_vOffsetScale;
    bool            m_bAbsolute;
    COLLIDER2D_TYPE m_Shape;

    Matrix          m_matCollider2D;    // Collider 의 월드행렬

public:
    virtual void finaltick() override;

public:
	void SetOffsetPos(Vec2 _vOffsetPos) { m_vOffsetPos = Vec3(_vOffsetPos.x, _vOffsetPos.y, 0.f); }
    void SetOffsetScale(Vec2 _vOffsetScale) { m_vOffsetScale = Vec3(_vOffsetScale.x, _vOffsetScale.y, 1.f); }
	void SetAbsolute(bool _bSet) { m_bAbsolute = _bSet; }

	void SetCollider2DType(COLLIDER2D_TYPE _Type) { m_Shape = _Type; }

public:
    CCollider2D();
    ~CCollider2D();

};

