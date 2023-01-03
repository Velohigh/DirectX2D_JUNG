#include "pch.h"
#include "CMonsterScript.h"


CMonsterScript::CMonsterScript()
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::tick()
{
}

void CMonsterScript::BeginOverlap(CCollider2D* _Other)
{
	if (L"Parent Object" == _Other->GetOwner()->GetName())
	{
		//Destroy();	

		SetLifeSpan(2.f);
	}
}

