#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

class CMaterial :
    public CRes
{
private:
    Ptr<CGraphicsShader>    m_pShader;  // Material 에서 셰이더 참조

    tMtrlConst              m_Const;    // 상수 값
    Ptr<CTexture>           m_arrTex[TEX_END];


public:    
    void SetScalarParam(SCALAR_PARAM _Param,void* _Src);  // Mtrl상수값을 세팅하는 함수, #1.어느 위치에 들어갈 것인지, #2.전달할 데이터의 시작 주소
    void SetTexParam(TEX_PARAM _Param, const Ptr<CTexture>& _Tex);  // 텍스쳐 세팅 함수

    void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }
    // 상수값을 디바이스에서 만들어둔 상수 버퍼로 옮긴 다음, b1 레지스터에 바인딩한다.
    virtual void UpdateData() override;

private:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }
public:
    virtual int Save(const wstring& _strRelativePath) { return S_OK; }

public:
    CMaterial();
    ~CMaterial();

};

