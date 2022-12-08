#include "pch.h"
#include "CConstBuffer.h"

#include "CDevice.h"


CConstBuffer::CConstBuffer(UINT _iRegistNum)
	: m_Desc{}
	, m_iRegistNum(_iRegistNum)
	, m_iElementSize(0)
	, m_iElementCount(0)
{
}

CConstBuffer::~CConstBuffer()
{
}

void CConstBuffer::Create(UINT _iElementSize, UINT _iElementCount)
{
	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;

	UINT iBufferSize = m_iElementSize * m_iElementCount;

	// 16����Ʈ ���� �޸� ����
	assert(!(iBufferSize % 16));

	// ��� ���� ����
	m_Desc.ByteWidth = m_iElementSize * m_iElementCount;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// ��� ���� �÷���
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;				// ������۴� �ý��� �޸𸮿��� �����Ͽ� ��������
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
	{
		assert(nullptr);
	}

}

void CConstBuffer::SetData(void* _pSrc, UINT _iSize)
{
	UINT size = _iSize;

	// ũ�Ⱑ �������� ���� �����ʹ� ������� ũ��� ����.
	if (0 == _iSize)
	{
		size = m_iElementSize * m_iElementCount;
	}

	// ��� ���� ũ�⺸�� �� ū �����Ͱ� �Է����� ���� ���.
	if (size > m_iElementSize * m_iElementCount)
	{
		assert(nullptr);
	}

	D3D11_MAPPED_SUBRESOURCE tSubRes = {};
	if (!FAILED(CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSubRes)))
	{
		// �������� �ý��۸޸𸮿� �����Ҵ��Ͽ� �����Ѵ�.
		memcpy(tSubRes.pData, _pSrc, _iSize);  // tSubRes.pData �������� ������, �����Ҵ�� ���� �ּ�
		CONTEXT->Unmap(m_CB.Get(), 0);	// �����Ҵ��Ͽ� ������ ���� ������ �������� GPU�� �ٽ� ������.
	}

}

void CConstBuffer::UpdateData()
{
	// GPU -> �������ͷ� ���ε�
	CONTEXT->VSSetConstantBuffers(m_iRegistNum, 1, m_CB.GetAddressOf());
}
