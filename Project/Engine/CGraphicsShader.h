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

    //BS_TYPE;
    //DS_TYPE;
public:
    void CreateVertexShader(const wstring& _strFileName, const string& _strFuncName);
    void CreatePixelShader(const wstring& _strFileName, const string& _strFuncName);
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_eTopology = _Topology; }
    void SetRSType(RS_TYPE _Type) { m_RSType = _Type; }
    virtual void UpdateData() override;     // 리소스 바인딩하는 함수, 이 셰이더를 사용하겠다!! 라는 거

public:
    CGraphicsShader();
    ~CGraphicsShader();
};

