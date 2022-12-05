#include "pch.h"
#include "Test.h"

#include "CDevice.h" // ����̽��� �������� ���� : GPU�� ����ϱ����� Device, Context�� ����ϱ� ����

#include "CPathMgr.h"

// ���� ����, ���� ������ ������ ����
ComPtr<ID3D11Buffer> g_VB;

// ���̴�
ComPtr<ID3DBlob>	g_VSBlob;
ComPtr<ID3DBlob>	g_PSBlob;
ComPtr<ID3DBlob>	g_ErrBlob;

ComPtr<ID3D11VertexShader>	g_VS;
ComPtr<ID3D11PixelShader>	g_PS;

// InputLayut
ComPtr<ID3D11InputLayout>	g_Layout;



void Init()
{
	Vtx arrVtx[3] = {};

	arrVtx[0].vPos = Vec3(0.f, 1.f, 0.5f);
	arrVtx[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);

	arrVtx[1].vPos = Vec3(1.f, -1.f, 0.5f);
	arrVtx[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);

	arrVtx[2].vPos = Vec3(-1.f, -1.f, 0.5f);
	arrVtx[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);


	// ���� Desc �� ä����, VertexBuffer �� ������.
	D3D11_BUFFER_DESC tBufferDesc = {};

	// ���� ����뵵
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// SystemMemory ���� ���� ������ ����
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // CPU���� �����Ҷ� ������� �������� �뵵

	// ���� ũ��
	tBufferDesc.ByteWidth = sizeof(Vtx) * 3; // GPU �޸𸮿� �Ҵ�

	// Buffer ����
	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = arrVtx;
	if (FAILED(DEVICE->CreateBuffer(&tBufferDesc, &tSub, g_VB.GetAddressOf())))
	{
		assert(nullptr);
	}

	// Shader ���� ���
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += L"shader\\test.fx";

	// VertexShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "VS_Test", "vs_5_0", 0, 0, g_VSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)g_ErrBlob->GetBufferPointer()
			, "Vertex Shader Compile Failed!!", MB_OK);
	}

	// PixelShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "PS_Test", "ps_5_0", 0, 0, g_PSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)g_ErrBlob->GetBufferPointer()
			, "Pixel Shader Compile Failed!!", MB_OK);
	}

	// �����ϵ� ��ü�� VertexShader, PixelShader �� �����.
	DEVICE->CreateVertexShader(g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize()
		, nullptr, g_VS.GetAddressOf());
	DEVICE->CreatePixelShader(g_PSBlob->GetBufferPointer(), g_PSBlob->GetBufferSize()
		, nullptr, g_PS.GetAddressOf());


	// InputLayout ����
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[2] = { };

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

	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 2
		, g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize()
		, g_Layout.GetAddressOf())))
	{
		assert(nullptr);
	}


}

void Tick()
{

}

void Render()
{
	// IA �ܰ迡�� ����� ���ؽ� ���� ����, �����ϴ� �ͻ��̰�, IA�ܰ谡 ����Ǵ°��� �ƴϴ�.
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;	// �������� Ư�� �������� �������ϰ� ������ ���, ���� X
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetInputLayout(g_Layout.Get());	// IA���� ���� ���̾ƿ��� ����
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 3���� ��� ������ ���ڴٴ� ����

	CONTEXT->VSSetShader(g_VS.Get(), nullptr, 0); // ���ؽ� ���̴� ����
	CONTEXT->PSSetShader(g_PS.Get(), nullptr, 0); // �ȼ� ���̴� ����

	// ���� ������ ���������� ���� �Լ�.
	CONTEXT->Draw(3, 0);
}

void Release()
{
}
