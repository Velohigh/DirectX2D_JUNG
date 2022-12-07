#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"


CMesh::CMesh()
	: CRes(RES_TYPE::MESH)
	, m_tVBDesc{}
	, m_VtxCount(0)
	, m_tIBDesc{}
	, m_IdxCount(0)
	, m_pVtxSys(nullptr)
	, m_pIdxSys(nullptr)
{
}

CMesh::~CMesh()
{
	if (nullptr != m_pVtxSys)
		delete m_pVtxSys;

	if (nullptr != m_pIdxSys)
		delete m_pIdxSys;

}

void CMesh::Create(void* _VtxSysmem, UINT _iVtxCount, void* _IdxSysmem, UINT _IdxCount)
{
	m_VtxCount = _iVtxCount;
	m_IdxCount = _IdxCount;

	// �Է����� ���� �ý��۸޸𸮴� ���������� �Ҹ��� ���̱� ������, ���� ũ��� �����Ҵ��Ͽ� �����صд�.
	// ���߿� GPU���� �ý��� �޸𸮷� �����Ͽ� �����̳� �ε��� ������ �ٽ� Ȯ�ΰ����ϱ� ����.
	m_pVtxSys = new Vtx[m_VtxCount];
	memcpy(m_pVtxSys, _VtxSysmem, sizeof(Vtx) * m_VtxCount);

	m_pIdxSys = new UINT[m_IdxCount];
	memcpy(m_pIdxSys, _IdxSysmem, sizeof(UINT) * m_IdxCount);


	// ���� Desc �� ä����, VertexBuffer �� ������.
	// ���� ����뵵
	m_tVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// SystemMemory ���� ���� ���� ����
	m_tVBDesc.CPUAccessFlags = 0;
	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT; // CPU���� �����Ҷ� ������� �������� �뵵

	// ���� ũ��
	m_tVBDesc.ByteWidth = sizeof(Vtx) * m_VtxCount; // GPU �޸𸮿� �Ҵ��� ũ��

	// Buffer ����
	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = _VtxSysmem;
	if (FAILED(DEVICE->CreateBuffer(&m_tVBDesc, &tSub, m_VB.GetAddressOf())))
	{
		assert(nullptr);
	}


	// ���� Desc �� ä����, IndexBuffer �� ������.
	// �ε��� ����뵵
	m_tIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// SystemMemory ���� ���� �Ұ���
	m_tIBDesc.CPUAccessFlags = 0;			// ���� X
	m_tIBDesc.Usage = D3D11_USAGE_DEFAULT; // CPU���� �����Ҷ� ������� �������� �뵵

	// �ε��� ���� ũ��
	m_tIBDesc.ByteWidth = sizeof(UINT) * m_IdxCount; // GPU �޸𸮿� �Ҵ��� ũ��

	// �ε��� ���� ����
	tSub.pSysMem = _IdxSysmem;
	if (FAILED(DEVICE->CreateBuffer(&m_tIBDesc, &tSub, m_IB.GetAddressOf()))) // �ε��� ���ۿ� ����� ������
	{
		assert(nullptr);
	}


}

void CMesh::UpdateData()
{
	// IA �ܰ迡�� ����� ���ؽ� ���� ����, �����ϴ� �ͻ��̰�, IA�ܰ谡 ����Ǵ°��� �ƴϴ�.
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;	// �������� Ư�� �������� �������ϰ� ������ ���, ������ ��� X
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);  // �ε��� ���� ����
}

void CMesh::render()
{
	UpdateData();

	// ���� ������ ���������� ���� �Լ�.
	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}
