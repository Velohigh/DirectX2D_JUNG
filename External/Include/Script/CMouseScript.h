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

private:
    CLONE(CMouseScript);
public:
    CMouseScript();
    ~CMouseScript();
};

