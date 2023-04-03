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
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11RenderTargetView>	    m_RTV;
    ComPtr<ID3D11DepthStencilView>	    m_DSV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    D3D11_TEXTURE2D_DESC                m_Desc;
    ScratchImage                        m_Image;
    const Image*                        m_pImage;

    UINT                                m_iRecentNum;
    Vec2                                m_Offset;

public:
    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat
        , UINT _BindFlag, D3D11_USAGE _Usage);

    int Create(ComPtr<ID3D11Texture2D> _tex2D);


public:
    float Width() { return (float)m_Desc.Width; }
    float Height() { return (float)m_Desc.Height; }
    ComPtr<ID3D11Texture2D>             GetTex2D() { return m_Tex2D; }
    ComPtr<ID3D11ShaderResourceView>    GetSRV() { return m_SRV; }
    ComPtr<ID3D11RenderTargetView>	    GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView>	    GetDSV() { return m_DSV; }
    ComPtr<ID3D11UnorderedAccessView>   GetUAV() { return m_UAV; }
    const Image* GetImage() { return m_pImage; }

	const Vec2& GetOffset() { return m_Offset; }
    void SetOffset(float _x, float _y) { m_Offset = Vec2(_x, _y); }
    void SetOffset(Vec2 _vec2) { m_Offset = _vec2; }

private:
    virtual int Load(const wstring& _strFilePath) override;
public:
    virtual int Save(const wstring& _strRelativePath) override;

public:
    // _PipelineStage : PIPELINE_STAGE
    void UpdateData(int _iRegisterNum, int _PipelineStage);
    void UpdateData_CS(int _iRegisterNum, bool _bShaderRes);
    void Clear();
    static void Clear(int _iRegisterNum);
    void Clear_CS(bool _bShaderRes);

public:
    int GetPixelColor(int x, int y)
    {
        //// 맵 바깥의 Pixel 을 달라고 하면 검은색(갈수 없는곳) 을 리턴한다.
        if (x < 0 || y < 0)
        {
            return RGB(0, 0, 0);
        }

        if (x >= Width() || y >= Height())
        {
            return RGB(0, 0, 0);
        }
        const uint8_t* pixels = m_pImage->pixels;
        const size_t pixelSize = m_pImage->slicePitch / Height();

        uint8_t* pixel = m_pImage->pixels + (y * m_pImage->rowPitch) + (x * 4);
        uint8_t r = pixel[0];
        uint8_t g = pixel[1];
        uint8_t b = pixel[2];
        uint8_t a = pixel[3];

        return RGB((int)r, (int)g, (int)b);
    }

    int GetPixelColor(Vec2 _vector)
    {
        return GetPixelColor((int)_vector.x, (int)_vector.y);
    }


private:
    virtual void UpdateData() override;

public:
    CTexture(bool _bEngine = false);
    ~CTexture();
};

