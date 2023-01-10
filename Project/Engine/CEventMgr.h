#pragma once
#include "CSingleton.h"



class CEventMgr :
    public CSingleton< CEventMgr>
{
    SINGLE(CEventMgr);
private:
    vector<tEvent>          m_vecEvent;    
    vector<CGameObject*>    m_vecGC;

public:
    void AddEvent(const tEvent& _evn) { m_vecEvent.push_back(_evn); }

private:
    void GC_Clear();

public:
    void tick();
};

