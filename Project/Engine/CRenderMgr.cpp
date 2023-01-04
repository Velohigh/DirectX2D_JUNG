#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CCamera.h"
#include "CLight2D.h"

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
    UpdateData();


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

void CRenderMgr::UpdateData()
{
    struct {
        tLightInfo  arrInfo[10];
        int         iLightCount;
        int         padding[3];
    }arrInfo{};

    for (size_t i = 0; i < m_vecLight2D.size(); ++i)
    {
        arrInfo.arrInfo[i] = m_vecLight2D[i];
    }
    arrInfo.iLightCount = m_vecLight2D.size();

    static CConstBuffer* pLightBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::LIGHT);
    pLightBuffer->SetData(&arrInfo, sizeof(arrInfo));
    pLightBuffer->UpdateData();
}

void CRenderMgr::Clear()
{
    m_vecLight2D.clear();

}
