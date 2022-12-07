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

	// 입력으로 들어온 시스템메모리는 지역변수라 소멸할 것이기 때문에, 같은 크기로 동적할당하여 복사해둔다.
	// 나중에 GPU에서 시스템 메모리로 접근하여 정점이나 인덱스 정보를 다시 확인가능하기 위해.
	m_pVtxSys = new Vtx[m_VtxCount];
	memcpy(m_pVtxSys, _VtxSysmem, sizeof(Vtx) * m_VtxCount);

	m_pIdxSys = new UINT[m_IdxCount];
	memcpy(m_pIdxSys, _IdxSysmem, sizeof(UINT) * m_IdxCount);


	// 버퍼 Desc 를 채워서, VertexBuffer 를 만들어낸다.
	// 정점 저장용도
	m_tVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// SystemMemory 에서 수정 가능 여부
	m_tVBDesc.CPUAccessFlags = 0;
	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT; // CPU에서 접근할때 어떤식으로 접근할지 용도

	// 버퍼 크기
	m_tVBDesc.ByteWidth = sizeof(Vtx) * m_VtxCount; // GPU 메모리에 할당할 크기

	// Buffer 생성
	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = _VtxSysmem;
	if (FAILED(DEVICE->CreateBuffer(&m_tVBDesc, &tSub, m_VB.GetAddressOf())))
	{
		assert(nullptr);
	}


	// 버퍼 Desc 를 채워서, IndexBuffer 를 만들어낸다.
	// 인덱스 저장용도
	m_tIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// SystemMemory 에서 수정 불가능
	m_tIBDesc.CPUAccessFlags = 0;			// 수정 X
	m_tIBDesc.Usage = D3D11_USAGE_DEFAULT; // CPU에서 접근할때 어떤식으로 접근할지 용도

	// 인덱스 버퍼 크기
	m_tIBDesc.ByteWidth = sizeof(UINT) * m_IdxCount; // GPU 메모리에 할당할 크기

	// 인덱스 버퍼 생성
	tSub.pSysMem = _IdxSysmem;
	if (FAILED(DEVICE->CreateBuffer(&m_tIBDesc, &tSub, m_IB.GetAddressOf()))) // 인덱스 버퍼에 결과값 얻어오기
	{
		assert(nullptr);
	}


}

void CMesh::UpdateData()
{
	// IA 단계에서 사용할 버텍스 버퍼 전달, 세팅하는 것뿐이고, IA단계가 실행되는것이 아니다.
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;	// 정점에서 특정 정점부터 렌더링하고 싶을떄 사용, 요즘은 사용 X
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);  // 인덱스 버퍼 세팅
}

void CMesh::render()
{
	UpdateData();

	// 실제 렌더링 파이프라인 실행 함수.
	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}
