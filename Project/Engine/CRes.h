#pragma once
#include "CEntity.h"

class CRes :
    public CEntity
{
private:
    const RES_TYPE  m_Type;
    int             m_iRefCount; // �� ���ҽ��� �󸶳� ���� ������κ��� �����ǰ� �ֳ� ����ϰ�, 0�� �Ǹ� ��������.

    wstring         m_strKey;       // ���ҽ� �Ŵ������� �����Ҷ� ã������ ������ Ű��
    wstring         m_strRelativePath;  // ���Ϸκ��� ���ҽ� �ε��ϱ� ���� ��� (bin\content\ ���ķ� �ٴ� ��θ� ���Ѵ�.)

private:
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strPath) { m_strRelativePath = _strPath; }

    void AddRef() { ++m_iRefCount; }
    void Release();

public:
    // ���ҽ� ���ε�
    virtual void UpdateData() = 0;

    // ���ҽ��� Clone �� �������� �ʴ´�.
    virtual CRes* Clone() { return nullptr; assert(nullptr); }

public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }

public:
    CRes(RES_TYPE _type);
    virtual ~CRes();

    friend class CResMgr;   // ���ҽ��� ���ҽ� �Ŵ����� ���ؼ��� ������ �� �ִ�.

    // �ڱ⸦ ����Ű�� ����Ʈ �������ʿ����� ���۷��������� ȣ������ �� �ֵ��� friend ����
    // ������ Ptr�� ���ø��̱� ������ �̷� �������� �ؾ�
    // Ptr ���ø����� �Ļ��Ǵ� ��� �ֵ鿡�� friend �������� �� �ִ�.
    template <typename T>
    friend class Ptr;
    
};

