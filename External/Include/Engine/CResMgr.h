#pragma once
#include "CSingleton.h"

#include "ptr.h"
#include "CMesh.h"

// ���ҽ� ���� �Ŵ���
class CResMgr :
    public CSingleton<CResMgr>
{
private:
    //map<wstring, Ptr<CRes>> m_mapRes;   // ���ҽ����� ����Ʈ �����ͷ� �����Ѵ�.
    map<wstring, Ptr<CRes>> m_arrRes[(UINT)RES_TYPE::END];  // ���ҽ����� �� + �迭�� �����Ѵ�. (���ҽ��� Ÿ�Ժ��� �����ϱ� ����.)

public:
    // ���ҽ� ��� ���
    void init();

private:
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();

public:
    // ���ҽ� Get �Լ�.
    // ��ȯŸ�Ե� Ptr�� �ؾ� �Ѵ�, ���� �޾ư��� �ʿ��� �Ϲ� *�����ͷ� �޾ƹ����� RefCount�� �����ȵǱ� ����.
    template<typename T>
    Ptr<T> FindRes(const wstring& _strKey);


public:
    CResMgr();
    ~CResMgr();

};

template<typename T>
inline Ptr<T> CResMgr::FindRes(const wstring& _strKey)
{
    // �Է����� ���� Ŭ���� id���� CMesh �� ������ m_arrRes[(UINT)CMesh] �迭���� ã���ش�.
    if (typeid(T).hash_code() == typeid(CMesh).hash_code())
    {
		map<wstring, Ptr<CMesh>>::iterator iter = m_arrRes[(UINT)RES_TYPE::MESH].find(_strKey);
        if (iter == m_arrRes[(UINT)RES_TYPE::MESH].end())
            return nullptr;

        return (CMesh*)iter->second.Get(); // Get�� ���ϸ� Ptr<CMesh> �� ��ȯ�ϱ� ������ ȣȯ�� �ȵȴ�.
    }


    return nullptr;
}
