#pragma once
#include "CRes.h"

class CShader :
    public CRes
{
protected:
    ComPtr<ID3DBlob>    m_ErrBlob;

    // 이 셰이더가 동작되는데 필요한 파라미터 목록을 추가할 예정.
    //

public:
    virtual int Save(const wstring& _strRelativePath) { return S_OK; }

private:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }

public:
    CShader(RES_TYPE _eType);
    ~CShader();
};

