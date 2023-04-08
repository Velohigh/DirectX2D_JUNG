#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2D.h"
#include "CMeshRender.h"

#include "ptr.h"
#include "CMaterial.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
{
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (nullptr != m_pCurAnim)
	{
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->Reset();
		}

		m_pCurAnim->finaltick();
	}
}

void CAnimator2D::UpdateData()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
	bool m_bFolder = m_pCurAnim->IsFolderTex();
	if (!m_bFolder)
	{
		const tAnim2DFrm& frm = m_pCurAnim->GetCurFrame();
		Vec2 vBackSize = m_pCurAnim->GetBackSize();

		int iAnimUse = 1;
		pMtrl->SetScalarParam(INT_0, &iAnimUse);
		pMtrl->SetScalarParam(VEC2_0, &frm.LeftTopUV);
		pMtrl->SetScalarParam(VEC2_1, &frm.SliceUV);
		pMtrl->SetScalarParam(VEC2_2, &frm.Offset);
		pMtrl->SetScalarParam(VEC2_3, &vBackSize);

		pMtrl->SetTexParam(TEX_0, m_pCurAnim->GetAtlasTex());
	}

	else if (m_bFolder)
	{
		vector<Ptr<CTexture>> vecTex = m_pCurAnim->GetFolderTex();

		int CurFrmCount = m_pCurAnim->GetCurFrmCount();
		Vec2 Offset = vecTex[CurFrmCount]->GetOffset();

		pMtrl->SetScalarParam(VEC2_2, &Offset);
		pMtrl->SetTexParam(TEX_0, vecTex[CurFrmCount]);
	}
}

void CAnimator2D::Clear()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();

	int iAnimUse = 0;
	Vec2 vec2 = {};
	pMtrl->SetScalarParam(INT_0, &iAnimUse);
	pMtrl->SetScalarParam(VEC2_0, &vec2);
	pMtrl->SetScalarParam(VEC2_1, &vec2);
	pMtrl->SetScalarParam(VEC2_2, &vec2);
	pMtrl->SetScalarParam(VEC2_3, &vec2);


	Ptr<CTexture> pTex = nullptr;
	pMtrl->SetTexParam(TEX_0, pTex);
}

void CAnimator2D::Play(const wstring& _strName, bool _bRepeat)
{
	CAnim2D* pAnim = FindAnim(_strName);
	assert(pAnim);

	m_pCurAnim = pAnim;
	m_bRepeat = _bRepeat;
}

CAnim2D* CAnimator2D::FindAnim(const wstring& _strName)
{
	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator2D::CreateAnimation(const wstring& _strAnimName
	, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize
	, int _FrameCount, int _FPS)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->Create(_strAnimName, _AtlasTex, _vLeftTop, _vSlice, _vBackSize, _FrameCount, _FPS);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_strAnimName, pAnim));
}

void CAnimator2D::CreateFolderAnimation(const wstring& _RelativePath, int _FrameCount, float _FPS)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->CreateFolderAnim(_RelativePath, _RelativePath, _FrameCount, _FPS);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_RelativePath, pAnim));
}

void CAnimator2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_bRepeat, sizeof(bool), 1, _File);

	size_t AnimCount = m_mapAnim.size();
	fwrite(&AnimCount, sizeof(size_t), 1, _File);

	for (const auto& pair : m_mapAnim)
	{
		pair.second->SaveToLevelFile(_File);
	}

	wstring strCurAnimName;
	if (nullptr != m_pCurAnim)
	{
		strCurAnimName = m_pCurAnim->GetName();
	}
	SaveWString(strCurAnimName, _File);
}

void CAnimator2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_bRepeat, sizeof(bool), 1, _File);

	size_t AnimCount = 0;
	fread(&AnimCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < AnimCount; ++i)
	{
		CAnim2D* pNewAnim = new CAnim2D;
		pNewAnim->LoadFromLevelFile(_File);

		m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
		pNewAnim->m_pOwner = this;
	}

	wstring strCurAnimName;
	LoadWString(strCurAnimName, _File);

	m_pCurAnim = FindAnim(strCurAnimName);
}

void CAnimator2D::Create_Player_Animation()
{
	this->CreateFolderAnimation(L"texture\\player\\spr_idle", 11, 10.f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_idle_to_run", 4, 25.f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_run", 10, 14.2857f);	// 0.07초 간격
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_run_to_idle", 5, 14.28f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_jump", 4, 11.428f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_fall", 4, 11.428f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_attack", 7, 35.714f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_landing", 5, 16.667f);	// 0.06초 간격
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_roll", 7, 22.222f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_player_playsong", 31, 8.857f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_hurtfly_begin", 2, 14.2857f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_hurtfly_loop", 4, 14.2857f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_hurtground", 6, 10.909f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_wallgrab", 1, 1.f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_player_flip", 11, 14.2857f);

	this->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_character", 1, 1.f);
}

void CAnimator2D::Create_Grunt_Animation()
{
	this->Animator2D()->CreateFolderAnimation(L"texture\\grunt\\spr_grunt_idle", 8, 9.1f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\grunt\\spr_grunt_walk", 10, 14.2857f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\grunt\\spr_grunt_run", 10, 14.2857f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\grunt\\spr_grunt_attack", 8, 14.2857f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\grunt\\spr_grunt_turn", 8, 16.6667f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\grunt\\spr_grunt_hurtfly", 2, 2.5f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\grunt\\spr_grunt_hurtground", 16, 16.6667f);

}

void CAnimator2D::Create_Effect_Animation()
{
	this->Animator2D()->CreateFolderAnimation(L"texture\\effect\\spr_dustcloud", 7, 16.6667f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\effect\\spr_jumpcloud", 4, 16.6667f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\effect\\spr_landcloud", 7, 16.6667f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\effect\\spr_slash", 5, 25.5102f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\effect\\spr_enemy_follow", 2, 25.5102f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\effect\\spr_effect_bloodanimation", 6, 15.f);
	this->Animator2D()->CreateFolderAnimation(L"texture\\effect\\spr_effect_bloodanimation2", 10, 15.f);
}
