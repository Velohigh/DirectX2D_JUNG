#pragma once
#include "CEntity.h"

class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>    m_SB;
    D3D11_BUFFER_DESC       m_tDesc;

    UINT                    m_iElementSize;
    UINT                    m_iElementCount;

public:
    void Create(UINT _iElementSize, UINT _iElementCount);
    void SetData(void* _pSrc, UINT _iSize = 0);

    // PIPELINE_STAGE
    void UpdateData(UINT _iRegisterNum, UINT _iPipeLineStage);

public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

