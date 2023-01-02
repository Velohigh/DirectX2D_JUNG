#pragma once
#include "CSingleton.h"

class CCamera;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    vector<CCamera*>            m_vecCam;
    vector<tDebugShapeInfo>     m_vecShapeInfo;

public:
    void init();
    void render();

public:
    int RegisterCamera(CCamera* _Cam, int _idx);
	void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_vecShapeInfo.push_back(_Info); }

    vector<tDebugShapeInfo>& GetDebugShapeInfo() { return m_vecShapeInfo; }

    CCamera* GetMainCam()
    {
        if (m_vecCam.empty())
            return nullptr;

        return m_vecCam[0];
    }

};

