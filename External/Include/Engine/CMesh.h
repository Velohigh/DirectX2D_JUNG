#pragma once

#include "CRes.h"

// 정점들의 정보를 리소스화하여 관리하는 클래스
class CMesh :
    public CRes
{
private:
    ComPtr<ID3D11Buffer>    m_VB;       // 정점 버퍼
    D3D11_BUFFER_DESC       m_tVBDesc;  // 정점 버퍼 디스크립션
    UINT                    m_VtxCount; // 정점 개수

    ComPtr<ID3D11Buffer>    m_IB;       // 인덱스 버퍼
    D3D11_BUFFER_DESC       m_tIBDesc;  // 인덱스 버퍼 디스크립션
    UINT                    m_IdxCount; // 인덱스 개수

    void*                   m_pVtxSys;      // 버텍스 버퍼 시스템메모리 주소
    void*                   m_pIdxSys;      // 인덱스 버퍼 시스템메모리 주소

public:
    void Create(void* _VtxSysmem, UINT _iVtxCount, void* _IdxSysmem, UINT _IdxCount);
    void render();

private:
    virtual void UpdateData() override; // 자신의 정점정보와, 인덱스 정보를 전달한다.

public:
    CMesh();
    ~CMesh();
};

