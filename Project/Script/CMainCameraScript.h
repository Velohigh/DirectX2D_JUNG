#pragma once
#include <Engine\CScript.h>

class CMainCameraScript :
    public CScript
{
private:
    float       m_fCamSpeed;
    bool        m_bFollowPlayer;
    Vec2        m_vMapsize;

public:
    virtual void tick() override;

    void SetMapsize(Vec2 _Size);
    void SetFollowPlayer(bool _bool);

private:

    CLONE(CMainCameraScript);
public:
    CMainCameraScript();
    ~CMainCameraScript();
};

