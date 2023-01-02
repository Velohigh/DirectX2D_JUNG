#pragma once
#include "CSingleton.h"



class CEventMgr :
    public CSingleton< CEventMgr>
{
    SINGLE(CEventMgr);
private:
    vector<tEvent>  m_vecEvent;

public:
	void AddEvent(const tEvent& _evn) { m_vecEvent.push_back(_evn); }

public:
    void tick();



};

