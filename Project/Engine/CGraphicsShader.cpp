#include "pch.h"
#include "CGraphicsShader.h"

#include "CPathMgr.h"
#include "CDevice.h"


CGraphicsShader::CGraphicsShader()
	: CShader(RES_TYPE::GRAPHICS_SHADER)
	, m_eTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

CGraphicsShader::~CGraphicsShader()
{
}

void CGraphicsShader::CreateVertexShader(const wstring& _strFileName, const string& _strFuncName)
{
	// Shader 파일 경로
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += _strFileName;

	// VertexShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "vs_5_0", 0, 0, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
			, "Vertex Shader Compile Failed!!", MB_OK);
	}

	// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
	DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, nullptr, m_VS.GetAddressOf());




	// InputLayout 생성
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[3] = { };

	LayoutDesc[0].SemanticName = "POSITION";
	LayoutDesc[0].SemanticIndex = 0;
	LayoutDesc[0].AlignedByteOffset = 0;	// 정점버퍼에서의 해당 데이터 시작 지점
	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // float3 개짜리 이므로 12바이트에 맞는 자료형 입력
	LayoutDesc[0].InputSlot = 0;			// 나중에 설명
	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[0].InstanceDataStepRate = 0;	// 안씀

	LayoutDesc[1].SemanticName = "COLOR";
	LayoutDesc[1].SemanticIndex = 0;
	LayoutDesc[1].AlignedByteOffset = 12;	// 정점버퍼에서의 해당 데이터 시작 지점
	LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	// 해당 레이아웃 데이터 크기
	LayoutDesc[1].InputSlot = 0;			// 나중에 설명
	LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[1].InstanceDataStepRate = 0;	// 안씀

	LayoutDesc[2].SemanticName = "TEXCOORD";
	LayoutDesc[2].SemanticIndex = 0;
	LayoutDesc[2].AlignedByteOffset = 28;	// 정점버퍼에서의 해당 데이터 시작 지점
	LayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;	// 해당 레이아웃 데이터 크기 8byte
	LayoutDesc[2].InputSlot = 0;			// 나중에 설명
	LayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[2].InstanceDataStepRate = 0;	// 안씀


	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 3
		, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, m_Layout.GetAddressOf())))
	{
		assert(nullptr);
	}

}

void CGraphicsShader::CreatePixelShader(const wstring& _strFileName, const string& _strFuncName)
{
	// Shader 파일 경로
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += _strFileName;

	// PixelShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0", 0, 0, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
			, "Pixel Shader Compile Failed!!", MB_OK);
	}

	// 컴파일된 객체로 PixelShader 를 만든다.
	DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize()
		, nullptr, m_PS.GetAddressOf());
}

void CGraphicsShader::UpdateData()
{
	CONTEXT->IASetInputLayout(m_Layout.Get());	// IA에서 사용될 레이아웃을 세팅
	CONTEXT->IASetPrimitiveTopology(m_eTopology); // 3개씩 끊어서 면으로 보겠다는 세팅

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0); // 버텍스 셰이더 세팅
	//CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0); // 훌 셰이더 세팅
	//CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0); // 도메인 셰이더 세팅
	//CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0); // 지오메트리 셰이더 세팅
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0); // 픽셀 셰이더 세팅

	CONTEXT->RSSetState(CDevice::GetInst()->GetRSState(m_RSType).Get());

}
