#pragma once

#include "CRes.h"

// �������� ������ ���ҽ�ȭ�Ͽ� �����ϴ� Ŭ����
class CMesh :
    public CRes
{
private:
    ComPtr<ID3D11Buffer>    m_VB;       // ���� ����
    D3D11_BUFFER_DESC       m_tVBDesc;  // ���� ���� ��ũ����
    UINT                    m_VtxCount; // ���� ����

    ComPtr<ID3D11Buffer>    m_IB;       // �ε��� ����
    D3D11_BUFFER_DESC       m_tIBDesc;  // �ε��� ���� ��ũ����
    UINT                    m_IdxCount; // �ε��� ����

    void*                   m_pVtxSys;      // ���ؽ� ���� �ý��۸޸� �ּ�
    void*                   m_pIdxSys;      // �ε��� ���� �ý��۸޸� �ּ�

public:
    void Create(void* _VtxSysmem, UINT _iVtxCount, void* _IdxSysmem, UINT _IdxCount);
    void render();

private:
    virtual void UpdateData() override; // �ڽ��� ����������, �ε��� ������ �����Ѵ�.

public:
    CMesh();
    ~CMesh();
};

