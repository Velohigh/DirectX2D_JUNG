#include "pch.h"
#include "CAnim2D.h"

CAnim2D::CAnim2D()
	: m_pOwner(nullptr)
{
}

CAnim2D::~CAnim2D()
{
}

void CAnim2D::finaltick()
{

}

void CAnim2D::Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex
	, Vec2 _vLeftTop, Vec2 _vSlice, int _FrameCount, int _FPS)
{
	assert(_AtlasTex.Get());

	SetName(_strAnimName);

	m_AtlasTex = _AtlasTex;

	for (size_t i = 0; i < _FrameCount; ++i)
	{
		tAnim2DFrm frm = {};

		frm.fDuration = 1.f / (float)_FPS;
		frm.LeftTopUV = Vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y);
		frm.SliceUV = _vSlice;

		m_vecFrm.push_back(frm);
	}
}