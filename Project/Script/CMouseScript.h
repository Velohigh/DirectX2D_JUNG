#pragma once
#include <Engine\CScript.h>

class CMouseScript :
    public CScript
{
private:
    Vector2	m_MousePos;			// ������ â������ ��ġ
    Vector2	m_MouseWorldPos;	// ������������� ���콺 ��ġ
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

