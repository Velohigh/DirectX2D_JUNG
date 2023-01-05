#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	: m_iElementSize(0)
	, m_iElementCount(0)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

void CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount)
{
	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;

	UINT iBufferSize = m_iElementSize * _iElementCount;

	// 16바이트 단위 메모리 정렬
	assert(!(iBufferSize % 16));

	// 상수버퍼 생성
	m_tDesc.ByteWidth = iBufferSize;
	m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	m_tDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&m_tDesc, nullptr, m_SB.GetAddressOf())))
	{
		assert(nullptr);
	}
}

void CStructuredBuffer::SetData(void* _pSrc, UINT _iSize)
{

}

void CStructuredBuffer::UpdateData(UINT _iRegisterNum, UINT _iPipeLineStage)
{

}