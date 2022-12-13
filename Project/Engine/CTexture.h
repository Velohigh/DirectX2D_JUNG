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
    ComPtr<ID3D11ShaderResourceView>    m_SRV;  // ���̴� ���ҽ� ��
    D3D11_TEXTURE2D_DESC                m_Desc; // ��ũ���� ����ü

    // ������ġ�� �̹����� �ý��� �޸𸮷� �������� Ŭ����, ��θ� �˷��ָ� �ȴ�!
    ScratchImage                        m_Image; 
                                    
    void* m_pSystem;    // 1�ܰ�� ���Ϸκ��� �ý��� �޸𸮷� �ؽ��� �����͸� �ҷ��´�.

private:
    virtual int Load(const wstring& _strFilePath) override; // �޸𸮷� �ε�
public:
    virtual int Save(const wstring& _strRelativePath) override;

public:
    // _PipelineStage : PIPELINE_STAGE
    void UpdateData(int _iRegisterNum, int _PipelineStage);     // �ؽ��ĸ� �������Ϳ� ���ε��ϴ� �Լ�.

private:
    virtual void UpdateData() override;

public:
    CTexture();
    ~CTexture();
};

