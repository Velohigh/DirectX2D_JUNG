#include "pch.h"
#include "CRes.h"

CRes::CRes(RES_TYPE _type)
	: m_Type(_type)
	, m_iRefCount(0)
{
}

CRes::~CRes()
{
}

void CRes::Release()
{
	--m_iRefCount;

	if (0 == m_iRefCount)
	{
		delete this;
	}
}
