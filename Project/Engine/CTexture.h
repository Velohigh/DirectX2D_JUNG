#pragma once
#include "CRes.h"

#include <DirectXTex\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex//DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex//DirectXTex")
#endif

class CTexture :
    public CRes
{
private:
    ComPtr<ID3D11Texture2D>             m_Tex2D;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;  // 셰이더 리소스 뷰
    D3D11_TEXTURE2D_DESC                m_Desc; // 디스크립션 구조체

    // 저장장치의 이미지를 시스템 메모리로 가져오는 클래스, 경로만 알려주면 된다!
    ScratchImage                        m_Image; 
                                    
    void* m_pSystem;    // 1단계로 파일로부터 시스템 메모리로 텍스쳐 데이터를 불러온다.

private:
    virtual int Load(const wstring& _strFilePath) override; // 메모리로 로딩
public:
    virtual int Save(const wstring& _strRelativePath) override;

public:
    // _PipelineStage : PIPELINE_STAGE
    void UpdateData(int _iRegisterNum, int _PipelineStage);     // 텍스쳐를 레지스터에 바인딩하는 함수.

private:
    virtual void UpdateData() override;

public:
    CTexture();
    ~CTexture();
};

