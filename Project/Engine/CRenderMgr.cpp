#include "pch.h"
#include "CRenderMgr.h"

#include "CCamera.h"

CRenderMgr::CRenderMgr()
{

}

CRenderMgr::~CRenderMgr()
{

}


void CRenderMgr::init()
{
}

void CRenderMgr::render()
{
    for (size_t i = 0; i < m_vecCam.size(); ++i)
    {
        if (nullptr == m_vecCam[i])
            continue;

        m_vecCam[i]->SortObject();
        m_vecCam[i]->render();
    }

    Clear();
}

int CRenderMgr::RegisterCamera(CCamera* _Cam, int _idx)
{
    if (m_vecCam.size() <= _idx)
    {
        m_vecCam.resize(_idx + 1);
    }

    m_vecCam[_idx] = _Cam;
    return _idx;
}

void CRenderMgr::Clear()
{
    m_vecLight2D.clear();

}
