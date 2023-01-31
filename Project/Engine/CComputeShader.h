#pragma once
#include "CShader.h"

class CComputeShader :
    public CShader
{
private:
    ComPtr<ID3D11ComputeShader>     m_CS;
    ComPtr<ID3DBlob>                m_CSBlob;

public:
    void CreateComputeShader(const wstring& _strFileName, const string& _strFuncName);

    void Dispatch(UINT _X, UINT _Y, UINT _Z);
    virtual void UpdateData();
    virtual void Clear();


    CLONE_DISABLE(CComputeShader);
public:
    CComputeShader();
    ~CComputeShader();
};

