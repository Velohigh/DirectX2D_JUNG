#pragma once
#include "CShader.h"

class CComputeShader :
    public CShader
{
private:
    ComPtr<ID3D11ComputeShader>     m_CS;

public:
    void CreateComputeShader(const wstring& _strFileName, const string& _strFuncName);
    virtual void UpdateData();
    virtual void Clear();


    CLONE_DISABLE(CComputeShader);
public:
    CComputeShader();
    ~CComputeShader();
};

