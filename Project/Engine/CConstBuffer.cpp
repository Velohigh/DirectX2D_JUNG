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

	// 16바이트 단위 메모리 정렬
	assert(!(iBufferSize % 16));

	// 상수 버퍼 생성
	m_Desc.ByteWidth = m_iElementSize * m_iElementCount;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 상수 버퍼 플래그
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;				// 상수버퍼는 시스템 메모리에서 접근하여 수정가능
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
	{
		assert(nullptr);
	}

}

void CConstBuffer::SetData(void* _pSrc, UINT _iSize)
{
	UINT size = _iSize;

	// 크기가 지정되지 않은 데이터는 상수버퍼 크기로 본다.
	if (0 == _iSize)
	{
		size = m_iElementSize * m_iElementCount;
	}

	// 상수 버퍼 크기보다 더 큰 데이터가 입력으로 들어온 경우.
	if (size > m_iElementSize * m_iElementCount)
	{
		assert(nullptr);
	}

	D3D11_MAPPED_SUBRESOURCE tSubRes = {};
	if (!FAILED(CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSubRes)))
	{
		// 맵핑으로 시스템메모리에 동적할당하여 복사한다.
		memcpy(tSubRes.pData, _pSrc, _iSize);  // tSubRes.pData 맵핑으로 가져온, 동적할당된 곳의 주소
		CONTEXT->Unmap(m_CB.Get(), 0);	// 동적할당하여 가져온 곳의 수정한 정보들을 GPU로 다시 보낸다.
	}

}

void CConstBuffer::UpdateData()
{
	// GPU -> 레지스터로 바인딩
	CONTEXT->VSSetConstantBuffers(m_iRegistNum, 1, m_CB.GetAddressOf());
}
