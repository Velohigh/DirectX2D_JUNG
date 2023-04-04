#include "pch.h"
#include "CAnim2D.h"

#include "CTimeMgr.h"
#include "CPathMgr.h"
#include "CResMgr.h"

CAnim2D::CAnim2D()
	: m_pOwner(nullptr)
	, m_iCurFrm(0)
	, m_bFinish(false)
	, m_fTime(0.f)
	, m_bFolderTex(false)
	, m_fDuration(0.f)
{
}

CAnim2D::~CAnim2D()
{
}

void CAnim2D::finaltick()
{	
	if (m_bFinish)
		return;

	m_fTime += DT;

	if (false == m_bFolderTex)
	{
		if (m_vecFrm[m_iCurFrm].fDuration < m_fTime)
		{
			m_fTime = m_fTime - m_vecFrm[m_iCurFrm].fDuration;
			++m_iCurFrm;

			if (m_vecFrm.size() <= m_iCurFrm)
			{
				m_iCurFrm = m_vecFrm.size() - 1;
				m_bFinish = true;
			}
		}

	}

	// 폴더 애니메이션일 경우
	else if (true == m_bFolderTex)
	{
		if (m_fDuration < m_fTime)
		{
			m_fTime = m_fTime - m_fDuration;
			++m_iCurFrm;

			if (m_vecFolderTex.size() <= m_iCurFrm)
			{
				m_iCurFrm = m_vecFolderTex.size() - 1;
				m_bFinish = true;
			}
		}
	}

}

void CAnim2D::Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex
	, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS)
{
	assert(_AtlasTex.Get());

	SetName(_strAnimName);

	m_AtlasTex = _AtlasTex;

	Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	m_vBackSize = _vBackSize / vResolution;

	for (size_t i = 0; i < _FrameCount; ++i)
	{
		tAnim2DFrm frm = {};

		frm.fDuration = 1.f / (float)_FPS;
		frm.LeftTopUV = Vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y) / vResolution;
		frm.SliceUV = _vSlice / vResolution;

		m_vecFrm.push_back(frm);
	}
}

void CAnim2D::CreateFolderAnim(const wstring& _strAnimName, const wstring& _RelativePath, int _FrameCount, float _FPS)
{
	m_bFolderTex = true;

	SetName(_strAnimName);

	wstring szPath = _RelativePath;

	m_fDuration = 1.f / (float)_FPS;

	m_vecFolderTex.clear();

	for (int i = 0; i < _FrameCount; ++i)
	{
		wstring Temp;
		Temp.resize(100);
		_itow_s(i, &Temp[0], 100, 10);

		wstring FilePath = szPath + L"\\" + Temp.c_str() + L".png";

		Ptr<CTexture> pTexture = CResMgr::GetInst()->Load<CTexture>(FilePath, FilePath);
		
		
		float _width = pTexture->Width();
		float _height = pTexture->Height();

		pTexture->SetOffset(0.f, -_height*2.f);
		pTexture->SetBackSize(200.f, 200.f);

		m_vecFolderTex.push_back(pTexture);
	}



}
void CAnim2D::SaveToLevelFile(FILE* _File)  // 원본
{
	fwrite(&m_bFolderTex, sizeof(bool), 1, _File);
	SaveWString(GetName(), _File);


	// 아틀라스 애니메이션 저장
	if (false == m_bFolderTex)
	{
		size_t FrameCount = m_vecFrm.size();
		fwrite(&FrameCount, sizeof(size_t), 1, _File);
		fwrite(m_vecFrm.data(), sizeof(tAnim2DFrm), FrameCount, _File);
		fwrite(&m_vBackSize, sizeof(Vec2), 1, _File);

		SaveResRef(m_AtlasTex.Get(), _File);
	}
 
	// 폴더 애니메이션 저장
	else if (true == m_bFolderTex)
	{
		size_t FrameCount = m_vecFolderTex.size();
		fwrite(&FrameCount, sizeof(size_t), 1, _File);

		fwrite(&m_fDuration, sizeof(float), 1, _File);

		// 이미지들 저장
		for (size_t i = 0; i < FrameCount; i++)
		{
			SaveResRef(m_vecFolderTex[i].Get(), _File);
		}
	}
}


void CAnim2D::LoadFromLevelFile(FILE* _File)
{
	bool isFolderAnim = false;
	fread(&isFolderAnim, sizeof(bool), 1, _File);

	wstring name;
	LoadWString(name, _File);
	SetName(name);

	if (false == isFolderAnim)
	{
		size_t FrameCount = 0;
		fread(&FrameCount, sizeof(size_t), 1, _File);

		for (size_t i = 0; i < FrameCount; ++i)
		{
			tAnim2DFrm frm = {};
			fread(&frm, sizeof(tAnim2DFrm), 1, _File);
			m_vecFrm.push_back(frm);
		}

		fread(&m_vBackSize, sizeof(Vec2), 1, _File);

		LoadResRef(m_AtlasTex, _File);
	}

	else if (true == isFolderAnim)
	{
		size_t FrameCount = 0;
		fread(&FrameCount, sizeof(size_t), 1, _File);

		fread(&m_fDuration, sizeof(float), 1, _File);

		m_vecFolderTex.resize(FrameCount);
		for (size_t i = 0; i < FrameCount; ++i)
		{
			LoadResRef(m_vecFolderTex[i], _File);
		}
		m_bFolderTex = true;
	}
}
