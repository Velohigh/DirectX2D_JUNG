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
    CShader(RES_TYPE _eType);
    ~CShader();
};

