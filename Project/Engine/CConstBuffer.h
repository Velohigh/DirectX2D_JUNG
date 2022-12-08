#pragma once
#include "CEntity.h"

// GPU의 b레지스터에 바인딩 할수 있는 버퍼
class CConstBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>    m_CB;	        // 상수 버퍼
    D3D11_BUFFER_DESC       m_Desc;         // 상수 버퍼 디스크립션
    const UINT              m_iRegistNum;   // 본인이 몇번 b레지스터에 바인딩 될것인지 지정.

    UINT                    m_iElementSize; // 개별 요소 크기
    UINT                    m_iElementCount;// 개별 요소 개수

public:
    void Create(UINT _iElementSize, UINT _iElementCount);   // 상수 버퍼 생성함수
    void SetData(void* _pSrc, UINT _iSize = 0); // 맵, 언맵으로 GPU의 상수 버퍼 값을 세팅.
    void UpdateData();      // 바인딩하는 함수, 이 상수 버퍼를 몇번 레지스터에 바인딩할것이냐


    CLONE_DISABLE(CConstBuffer);            // 상수버퍼는 클론기능 필요 X
public:
    CConstBuffer(UINT _iRegistNum);
    ~CConstBuffer();

};

