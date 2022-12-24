#pragma once
#include "CSingleton.h"

class CLevel;

class CLevelMgr :
    public CSingleton<CLevelMgr>
{
    SINGLE(CLevelMgr);

private:
    CLevel*     m_pCurLevel;    // 현재 지정된 레벨

public:
    CLevel* GetCurLevel() { return m_pCurLevel; }

public:
    void init();
    void tick();

};

