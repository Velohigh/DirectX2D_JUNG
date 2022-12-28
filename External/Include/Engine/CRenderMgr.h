#pragma once
#include "CSingleton.h"

class CCamera;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    vector<CCamera*>    m_vecCam;


public:
    void init();
    void render();



public:
    int RegisterCamera(CCamera* _Cam, int _idx);

    CCamera* GetMainCam()
    {
        if (m_vecCam.empty())
            return nullptr;

        return m_vecCam[0];
    }

};

