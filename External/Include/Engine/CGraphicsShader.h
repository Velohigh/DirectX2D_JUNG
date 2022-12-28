#pragma once
#include "CShader.h"

class CGraphicsShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>                m_VSBlob;
    ComPtr<ID3DBlob>                m_HSBlob;
    ComPtr<ID3DBlob>                m_DSBlob;
    ComPtr<ID3DBlob>                m_GSBlob;
    ComPtr<ID3DBlob>                m_PSBlob;

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    ComPtr<ID3D11InputLayout>	    m_Layout;       // 정점하나가 어떻게 구성되어있는지 알려주는 거  
    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;    // 정점을 몇개씩 끊어서 어떤 형태로 볼것인지 정하는 거
    
    RS_TYPE                         m_RSType;       // Rasterrizer 상태값
    DS_TYPE                         m_DSType;       // DepthStencil State
    BS_TYPE                         m_BSType;       // Blending State

    SHADER_DOMAIN                   m_Domain;       // 도메인 ( 분류 )

public:
    void CreateVertexShader(const wstring& _strFileName, const string& _strFuncName);
    void CreatePixelShader(const wstring& _strFileName, const string& _strFuncName);
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_eTopology = _Topology; }
    void SetRSType(RS_TYPE _Type) { m_RSType = _Type; }
    void SetDSType(DS_TYPE _Type) { m_DSType = _Type; }
    void SetBSType(BS_TYPE _Type) { m_BSType = _Type; }
    void SetDomain(SHADER_DOMAIN _domain) { m_Domain = _domain; }

    SHADER_DOMAIN GetDomain() { return m_Domain; }
    virtual void UpdateData() override;     // 리소스 바인딩하는 함수, 이 셰이더를 사용하겠다!! 라는 거

public:
    CGraphicsShader();
    ~CGraphicsShader();
};

