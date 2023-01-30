#pragma once
#include "ComponentUI.h"

class CTexture;
class CAnim2D;
class Animator2DUI :
    public ComponentUI
{
private:
    Ptr<CTexture>       m_pTexture;
    char                szBuff[50];
    vector<string>      m_vecStrData;
    int                 m_iSelectedIdx;
    bool                m_bcheck;

    Vec2    vLT;
    Vec2    vSlice;
    Vec2    vOffSet;

    CAnim2D* SelectAnim;

public:
    virtual int render_update() override;

public:
    void AddItem(const string& _strItem) { m_vecStrData.push_back(_strItem); }
    //void Clear() { m_vecStrData.clear(); }


public:
    Animator2DUI();
    ~Animator2DUI();
};

