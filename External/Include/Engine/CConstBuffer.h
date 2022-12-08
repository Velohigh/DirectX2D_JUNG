#pragma once
#include "CEntity.h"

// GPU�� b�������Ϳ� ���ε� �Ҽ� �ִ� ����
class CConstBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>    m_CB;	        // ��� ����
    D3D11_BUFFER_DESC       m_Desc;         // ��� ���� ��ũ����
    const UINT              m_iRegistNum;   // ������ ��� b�������Ϳ� ���ε� �ɰ����� ����.

    UINT                    m_iElementSize; // ���� ��� ũ��
    UINT                    m_iElementCount;// ���� ��� ����

public:
    void Create(UINT _iElementSize, UINT _iElementCount);   // ��� ���� �����Լ�
    void SetData(void* _pSrc, UINT _iSize = 0); // ��, ������� GPU�� ��� ���� ���� ����.
    void UpdateData();      // ���ε��ϴ� �Լ�, �� ��� ���۸� ��� �������Ϳ� ���ε��Ұ��̳�


    CLONE_DISABLE(CConstBuffer);            // ������۴� Ŭ�б�� �ʿ� X
public:
    CConstBuffer(UINT _iRegistNum);
    ~CConstBuffer();

};

