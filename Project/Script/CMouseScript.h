#pragma once
#include <Engine\CScript.h>

class CMouseScript :
    public CScript
{
private:
    Vector2	m_MousePos;			// 윈도우 창에서의 위치
    Vector2	m_MouseWorldPos;	// 월드공간에서의 마우스 위치
    Vector2 m_MouseMove;
    bool	m_ShowCursor;


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    Vector2 GetMouseWorldPos() { return m_MouseWorldPos; }

    void ComputeWorldMousePos(const Vec2& _CameraPos)
    {
        m_MouseWorldPos = m_MousePos + _CameraPos;
    }
private:
    CLONE(CMouseScript);
public:
    CMouseScript();
    ~CMouseScript();
};

