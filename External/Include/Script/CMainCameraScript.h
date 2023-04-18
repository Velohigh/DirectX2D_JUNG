#pragma once
#include <Engine\CScript.h>

class CMainCameraScript :
    public CScript
{
private:
    float       m_fCamSpeed;
    bool        m_bFollowPlayer;
    Vec2        m_vMapsize;

    Vec2        m_vStartPos;
    Vec2        m_vCameraShakeValue;
    bool        m_bCameraShakeOn;
    float       m_fCameraShakeTime;


public:
    virtual void tick() override;

    void SetMapsize(Vec2 _Size);
    void SetFollowPlayer(bool _bool);

    void SetCameraShakeOn(bool _bool);
    void SetPos(Vec2 _vec2);
    void SetPos(Vec3 _vec3);

private:
    void CameraShakeEffect();

private:

    CLONE(CMainCameraScript);
public:
    CMainCameraScript();
    ~CMainCameraScript();
};

