#pragma once
#include "CRes.h"

class CShader :
    public CRes
{
protected:
    ComPtr<ID3DBlob>    m_ErrBlob;

    // �� ���̴��� ���۵Ǵµ� �ʿ��� �Ķ���� ����� �߰��� ����.
    //

public:
    virtual int Save(const wstring& _strRelativePath) { return S_OK; }

private:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }

public:
    CShader(RES_TYPE _eType);
    ~CShader();
};

