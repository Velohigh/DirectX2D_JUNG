#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"

// ���ҽ� ���� �Ŵ���
class CResMgr :
    public CSingleton<CResMgr>
{
    SINGLE(CResMgr)


private:
    //map<wstring, Ptr<CRes>> m_mapRes;   // ���ҽ����� ����Ʈ �����ͷ� �����Ѵ�.
    map<wstring, Ptr<CRes>> m_arrRes[(UINT)RES_TYPE::END];  // ���ҽ����� �� + �迭�� �����Ѵ�. (���ҽ��� Ÿ�Ժ��� �����ϱ� ����.)

public:
    // ���ҽ� ��� ���
    void init();

private:
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();
    void LoadDefaultTexture();

public:
    // ���ҽ� Get �Լ�.
    // ��ȯŸ�Ե� Ptr�� �ؾ� �Ѵ�, ���� �޾ư��� �ʿ��� �Ϲ� *�����ͷ� �޾ƹ����� RefCount�� �����ȵǱ� ����.
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
    // Ŭ���� Ÿ�Կ� �´� ������ ���ҽ� Ÿ�Ժ��� �̸� �־�д�.
    const type_info& mesh = typeid(CMesh);
    //const type_info& meshdata = typeid(CMeshData);
    //const type_info& material = typeid(CMaterial);
    const type_info& texture = typeid(CTexture);
    //const type_info& sound = typeid(CSound);
    //const type_info& prefab = typeid(CPrefab);
    const type_info& gs = typeid(CGraphicsShader);
    //const type_info& cs = typeid(CComputeShader);


    // �Է����� ���� �ؽ� �ڵ尡 ��ġ�ϸ� �ش� �ؽ� �ڵ带 ��ȯ�Ѵ�.
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
    // �Է����� ���� T�� ���ҽ� Ÿ���� ã�ƿ´�.
    RES_TYPE type = GetResType<T>();

    // �Է����� ���� T�� ���ҽ� Ÿ���� �� �迭���� Ű������ �˻�
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)type].find(_strKey);
	if (iter == m_arrRes[(UINT)type].end())
		return nullptr;

	return (T*)iter->second.Get(); // Get�� ���ϸ� Ptr<CMesh> �� ��ȯ�ϱ� ������ ȣȯ�� �ȵȴ�.
}

template<typename T>
inline void CResMgr::AddRes(const wstring& _strKey, Ptr<T>& _Res)
{
    // �Ȱ��� Ű�� �̹� ��ϵǾ� �������� �ִ�. Map�� �ߺ�Ű�� ������� �ʱ⶧���� ������ ���� �� �ִ�.
    // �ߺ�Ű�� ���ҽ� �߰��Ϸ��� ���
    assert(!FindRes<T>(_strKey).Get());

    RES_TYPE type = GetResType<T>();
    m_arrRes[(UINT)type].insert(make_pair(_strKey, _Res.Get()));
    _Res->SetKey(_strKey);
}

template<typename T>
inline Ptr<T> CResMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{

    Ptr<CRes> pRes = FindRes<T>(_strKey).Get();

    // �ش� ����� ���ҽ��� �ش� Ű������ ��
    // �̹� ���� Ű�� �ִ� ���, ������ �ƴ϶� �̹� �ִ� ���ҽ��̹Ƿ� �ش� ���ҽ��� ��ȯ�ϸ� �ȴ�.
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
