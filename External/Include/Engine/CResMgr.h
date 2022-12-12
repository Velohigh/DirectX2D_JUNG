#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CMesh.h"
#include "CTexture.h"
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
    void LoadDefaultTexture();

public:
    // 리소스 Get 함수.
    // 반환타입도 Ptr로 해야 한다, 만약 받아가는 쪽에서 일반 *포인터로 받아버리면 RefCount가 관리안되기 때문.
    template<typename T>
    Ptr<T> FindRes(const wstring& _strKey);

    template<typename T>
    void AddRes(const wstring& _strKey, Ptr<T>& _Res);

    template<typename T>
    Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);

};

template <typename T>
RES_TYPE GetResType()
{
    // 클래스 타입에 맞는 정보를 리소스 타입별로 미리 넣어둔다.
    const type_info& mesh = typeid(CMesh);
    //const type_info& meshdata = typeid(CMeshData);
    //const type_info& material = typeid(CMaterial);
    const type_info& texture = typeid(CTexture);
    //const type_info& sound = typeid(CSound);
    //const type_info& prefab = typeid(CPrefab);
    const type_info& gs = typeid(CGraphicsShader);
    //const type_info& cs = typeid(CComputeShader);


    // 입력으로 들어온 해쉬 코드가 일치하면 해당 해쉬 코드를 반환한다.
    if (typeid(T).hash_code() == mesh.hash_code())
        return RES_TYPE::MESH;
    if (typeid(T).hash_code() == gs.hash_code())
        return RES_TYPE::GRAPHICS_SHADER;
    if (typeid(T).hash_code() == texture.hash_code())
        return RES_TYPE::TEXTURE;


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

template<typename T>
inline void CResMgr::AddRes(const wstring& _strKey, Ptr<T>& _Res)
{
    // 똑같은 키가 이미 등록되어 있을수도 있다. Map은 중복키를 허용하지 않기때문에 오류가 생길 수 있다.
    // 중복키로 리소스 추가하려는 경우
    assert(!FindRes<T>(_strKey).Get());

    RES_TYPE type = GetResType<T>();
    m_arrRes[(UINT)type].insert(make_pair(_strKey, _Res.Get()));
    _Res->SetKey(_strKey);
}

template<typename T>
inline Ptr<T> CResMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{

    Ptr<CRes> pRes = FindRes<T>(_strKey).Get();

    // 해당 경로의 리소스를 해당 키값으로 줘
    // 이미 같은 키가 있는 경우, 오류가 아니라 이미 있는 리소스이므로 해당 리소스를 반환하면 된다.
    if (nullptr != pRes)
        return (T*)pRes.Get();
    
    pRes = new T;
    pRes->SetKey(_strKey);
    pRes->SetRelativePath(_strRelativePath);

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += _strRelativePath;

    if (FAILED(pRes->Load(strFilePath)))
    {
        assert(nullptr);
    }

    RES_TYPE type = GetResType<T>();
    m_arrRes[(UINT)type].insert(make_pair(_strKey, pRes));

    return (T*)pRes.Get();
}
