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
	// Shader ���� ���
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += _strFileName;

	// VertexShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "vs_5_0", 0, 0, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
			, "Vertex Shader Compile Failed!!", MB_OK);
	}

	// �����ϵ� ��ü�� VertexShader, PixelShader �� �����.
	DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, nullptr, m_VS.GetAddressOf());




	// InputLayout ����
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[3] = { };

	LayoutDesc[0].SemanticName = "POSITION";
	LayoutDesc[0].SemanticIndex = 0;
	LayoutDesc[0].AlignedByteOffset = 0;	// �������ۿ����� �ش� ������ ���� ����
	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // float3 ��¥�� �̹Ƿ� 12����Ʈ�� �´� �ڷ��� �Է�
	LayoutDesc[0].InputSlot = 0;			// ���߿� ����
	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[0].InstanceDataStepRate = 0;	// �Ⱦ�

	LayoutDesc[1].SemanticName = "COLOR";
	LayoutDesc[1].SemanticIndex = 0;
	LayoutDesc[1].AlignedByteOffset = 12;	// �������ۿ����� �ش� ������ ���� ����
	LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	// �ش� ���̾ƿ� ������ ũ��
	LayoutDesc[1].InputSlot = 0;			// ���߿� ����
	LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[1].InstanceDataStepRate = 0;	// �Ⱦ�

	LayoutDesc[2].SemanticName = "TEXCOORD";
	LayoutDesc[2].SemanticIndex = 0;
	LayoutDesc[2].AlignedByteOffset = 28;	// �������ۿ����� �ش� ������ ���� ����
	LayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;	// �ش� ���̾ƿ� ������ ũ�� 8byte
	LayoutDesc[2].InputSlot = 0;			// ���߿� ����
	LayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[2].InstanceDataStepRate = 0;	// �Ⱦ�


	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 3
		, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, m_Layout.GetAddressOf())))
	{
		assert(nullptr);
	}

}

void CGraphicsShader::CreatePixelShader(const wstring& _strFileName, const string& _strFuncName)
{
	// Shader ���� ���
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += _strFileName;

	// PixelShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0", 0, 0, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer()
			, "Pixel Shader Compile Failed!!", MB_OK);
	}

	// �����ϵ� ��ü�� PixelShader �� �����.
	DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize()
		, nullptr, m_PS.GetAddressOf());

}

void CGraphicsShader::UpdateData()
{

	CONTEXT->IASetInputLayout(m_Layout.Get());	// IA���� ���� ���̾ƿ��� ����
	CONTEXT->IASetPrimitiveTopology(m_eTopology); // 3���� ��� ������ ���ڴٴ� ����

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0); // ���ؽ� ���̴� ����
	//CONTEXT->HSSetShader(m_VS.Get(), nullptr, 0); // �� ���̴� ����
	//CONTEXT->DSSetShader(m_VS.Get(), nullptr, 0); // ������ ���̴� ����
	//CONTEXT->GSSetShader(m_VS.Get(), nullptr, 0); // ������Ʈ�� ���̴� ����
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0); // �ȼ� ���̴� ����
}