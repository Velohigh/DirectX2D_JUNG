#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"

// 리소스 관리 매니저
class CResMgr :
    public CSingleton<CResMgr>
{
    SINGLE(CResMgr)


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

};

template <typename T>
RES_TYPE GetResType()
{
    // 클래스 타입에 맞는 정보를 리소스 타입별로 미리 넣어둔다.
    const type_info& mesh = typeid(CMesh);
    //const type_info& meshdata = typeid(CMeshData);
    //const type_info& material = typeid(CMaterial);
    //const type_info& texture = typeid(CTexture);
    //const type_info& sound = typeid(CSound);
    //const type_info& prefab = typeid(CPrefab);
    const type_info& gs = typeid(CGraphicsShader);
    //const type_info& cs = typeid(CComputeShader);


    // 입력으로 들어온 해쉬 코드가 일치하면 해당 해쉬 코드를 반환한다.
    if (typeid(T).hash_code() == mesh.hash_code())
        return RES_TYPE::MESH;
    if (typeid(T).hash_code() == gs.hash_code())
        return RES_TYPE::GRAPHICS_SHADER;

    return RES_TYPE::END;
}


template<typename T>
inline Ptr<T> CResMgr::FindRes(const wstring& _strKey)
{
    // 입력으로 들어온 T의 리소스 타입을 찾아온다.
    RES_TYPE type = GetResType<T>();

    // 입력으로 들어온 T의 리소스 타입의 맵 배열에서 키값으로 검색
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)type].find(_strKey);
	if (iter == m_arrRes[(UINT)type].end())
		return nullptr;

	return (T*)iter->second.Get(); // Get을 안하면 Ptr<CMesh> 로 반환하기 떄문에 호환이 안된다.
}
