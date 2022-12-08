#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CMesh.h"

// 리소스 관리 매니저
class CResMgr :
    public CSingleton<CResMgr>
{
private:
    //map<wstring, Ptr<CRes>> m_mapRes;   // 리소스들을 스마트 포인터로 관리한다.
    map<wstring, Ptr<CRes>> m_arrRes[(UINT)RES_TYPE::END];  // 리소스들을 맵 + 배열로 관리한다. (리소스를 타입별로 구분하기 위함.)

public:
    // 리소스 등록 기능
    void init();

private:
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();

public:
    // 리소스 Get 함수.
    // 반환타입도 Ptr로 해야 한다, 만약 받아가는 쪽에서 일반 *포인터로 받아버리면 RefCount가 관리안되기 때문.
    template<typename T>
    Ptr<T> FindRes(const wstring& _strKey);


public:
    CResMgr();
    ~CResMgr();

};

template<typename T>
inline Ptr<T> CResMgr::FindRes(const wstring& _strKey)
{
    // 입력으로 들어온 클래스 id값이 CMesh 와 같으면 m_arrRes[(UINT)CMesh] 배열에서 찾아준다.
    if (typeid(T).hash_code() == typeid(CMesh).hash_code())
    {
		map<wstring, Ptr<CMesh>>::iterator iter = m_arrRes[(UINT)RES_TYPE::MESH].find(_strKey);
        if (iter == m_arrRes[(UINT)RES_TYPE::MESH].end())
            return nullptr;

        return (CMesh*)iter->second.Get(); // Get을 안하면 Ptr<CMesh> 로 반환하기 떄문에 호환이 안된다.
    }


    return nullptr;
}
