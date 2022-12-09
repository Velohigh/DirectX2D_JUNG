#pragma once
#include "CEntity.h"

class CRes :
    public CEntity
{
private:
    const RES_TYPE  m_Type;
    int             m_iRefCount; // 이 리소스가 얼마나 많은 대상으로부터 참조되고 있나 계산하고, 0이 되면 지워진다.

    wstring         m_strKey;       // 리소스 매니저에서 관리할때 찾기위한 고유의 키값
    wstring         m_strRelativePath;  // 파일로부터 리소스 로딩하기 위한 경로 (bin\content\ 이후로 붙는 경로를 뜻한다.)

private:
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strPath) { m_strRelativePath = _strPath; }

    void AddRef() { ++m_iRefCount; }
    void Release();

public:
    // 리소스 바인딩
    virtual void UpdateData() = 0;

    // 리소스는 Clone 을 구현하지 않는다.
    virtual CRes* Clone() { return nullptr; assert(nullptr); }

public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }

public:
    CRes(RES_TYPE _type);
    virtual ~CRes();

    friend class CResMgr;   // 리소스는 리소스 매니저를 통해서만 관리할 수 있다.

    // 자기를 가리키는 스마트 포인터쪽에서도 레퍼런스해제를 호출해줄 수 있도록 friend 설정
    // 하지만 Ptr이 템플릿이기 때문에 이런 문법으로 해야
    // Ptr 템플릿으로 파생되는 모든 애들에게 friend 선언해줄 수 있다.
    template <typename T>
    friend class Ptr;
    
};

