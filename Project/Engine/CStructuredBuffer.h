#pragma once
#include "CEntity.h"

class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    D3D11_BUFFER_DESC                   m_tDesc;

    UINT                                m_iElementSize;
    UINT                                m_iElementCount;

    SB_TYPE                             m_Type;

public:
    void Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _Type);
    void SetData(void* _pSrc, UINT _iSize = 0);

    // PIPELINE_STAGE
    void UpdateData(UINT _iRegisterNum, UINT _iPipeLineStage);

    UINT GetElementSize() { return m_iElementSize; }
    UINT GetElementCount() { return m_iElementCount; }



    CLONE_DISABLE(CStructuredBuffer);
public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

